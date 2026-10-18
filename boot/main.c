#include <efi.h>
#include <efilib.h>

// --- Serial Port (COM1) Definitions ---
#define COM1_PORT 0x3F8

static inline void outb(UINT16 port, UINT8 data) {
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

static inline UINT8 inb(UINT16 port) {
    UINT8 ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void InitSerial() {
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x80);
    outb(COM1_PORT + 0, 0x03);
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x03);
    outb(COM1_PORT + 2, 0xC7);
    outb(COM1_PORT + 4, 0x0B);
}

void SerialWrite(const char* str) {
    while (*str) {
        while ((inb(COM1_PORT + 5) & 0x20) == 0);
        outb(COM1_PORT, *str++);
    }
}

// --- Framebuffer (GOP) Initialization ---
EFI_GRAPHICS_OUTPUT_PROTOCOL* InitGOP() {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    
    EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if (!EFI_ERROR(status)) {
        SerialWrite("[*] Graphics Output Protocol (GOP) Initialized.\n");
        Print(L"[*] Framebuffer detected: %dx%d\n\r", 
              gop->Mode->Info->HorizontalResolution, 
              gop->Mode->Info->VerticalResolution);
    }
    return gop;
}

// --- File Handling ---
EFI_FILE_HANDLE GetVolume(EFI_HANDLE image) {
    EFI_LOADED_IMAGE *loaded_image = NULL;
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_FILE_IO_INTERFACE *fs = NULL;
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE root = NULL;

    uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void **)&loaded_image);
    uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsGuid, (void **)&fs);
    uefi_call_wrapper(fs->OpenVolume, 2, fs, &root);
    
    return root;
}

// --- Diagnostics ---
void RunDiagnostics(EFI_SYSTEM_TABLE *SystemTable) {
    SerialWrite("[Diag] Running pre-boot diagnostics...\n");
    
    Print(L"[*] Firmware Vendor: %s\n\r", SystemTable->FirmwareVendor);
    Print(L"[*] Firmware Revision: %x\n\r", SystemTable->FirmwareRevision);
    
    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    UINTN MapKey, DescriptorSize;
    UINT32 DescriptorVersion;
    
    EFI_STATUS Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    
    // We expect EFI_BUFFER_TOO_SMALL because we passed a 0-size buffer. 
    // This gives us the exact size needed to allocate the real map later.
    if (Status == EFI_BUFFER_TOO_SMALL) {
        SerialWrite("[Diag] Memory map query successful.\n");
        Print(L"[*] Memory Map Size: %d bytes\n\r", MemoryMapSize);
    } else {
        SerialWrite("[Diag] Warning: Unexpected memory map status.\n");
    }
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    InitSerial();
    SerialWrite("\n\n=== OS Project UEFI Bootloader ===\n");
    Print(L"=== OS Project UEFI Bootloader ===\n\r");
    
    // Run hardware/memory diagnostics
    RunDiagnostics(SystemTable);
    
    InitGOP();

    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    if (!Volume) {
        SerialWrite("[!] Error: Could not mount EFI partition.\n");
        return EFI_LOAD_ERROR;
    }

    EFI_FILE_HANDLE KernelFile;
    if (!EFI_ERROR(uefi_call_wrapper(Volume->Open, 5, Volume, &KernelFile, L"EFI\\BOOT\\bzImage", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY))) {
        SerialWrite("[*] Kernel file located.\n");
        Print(L"[*] Kernel file located.\n\r");
        uefi_call_wrapper(KernelFile->Close, 1, KernelFile);
    }

    EFI_FILE_HANDLE ConfigFile;
    if (!EFI_ERROR(uefi_call_wrapper(Volume->Open, 5, Volume, &ConfigFile, L"EFI\\BOOT\\boot.cfg", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY))) {
        UINTN BufferSize = 512;
        CHAR8 Buffer[512];
        uefi_call_wrapper(ConfigFile->Read, 3, ConfigFile, &BufferSize, Buffer);
        Buffer[BufferSize] = '\0';
        
        CHAR8 *cmd_start = NULL;
        for (UINTN i = 0; i < BufferSize - 8; i++) {
            if (Buffer[i] == 'c' && Buffer[i+1] == 'm' && Buffer[i+2] == 'd' && 
                Buffer[i+3] == 'l' && Buffer[i+4] == 'i' && Buffer[i+5] == 'n' && 
                Buffer[i+6] == 'e' && Buffer[i+7] == '=') {
                cmd_start = &Buffer[i+8];
                break;
            }
        }
        if (cmd_start) {
            for (UINTN i = 0; cmd_start[i] != '\0'; i++) {
                if (cmd_start[i] == '\n' || cmd_start[i] == '\r') { cmd_start[i] = '\0'; break; }
            }
            SerialWrite("[+] Command Line Extracted.\n");
        }
        uefi_call_wrapper(ConfigFile->Close, 1, ConfigFile);
    }

    SerialWrite("[*] Halting CPU...\n");
    while (1) { __asm__ __volatile__("hlt"); }

    return EFI_SUCCESS;
}
