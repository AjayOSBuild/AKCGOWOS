#include <efi.h>
#include <efilib.h>

#define COM1_PORT 0x3F8

EFI_FILE_HANDLE LogFile = NULL;

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

UINTN StringLength(const char* str) {
    UINTN len = 0;
    while (str[len] != '\0') len++;
    return len;
}

// Unified Logging Function
void SystemLog(const char* narrow_msg, const CHAR16* wide_msg) {
    // 1. Write to Headless Serial (COM1)
    if (narrow_msg) {
        SerialWrite(narrow_msg);
        SerialWrite("\n");
        
        // 2. Write to Persistent Disk Log
        if (LogFile) {
            UINTN len = StringLength(narrow_msg);
            uefi_call_wrapper(LogFile->Write, 3, LogFile, &len, (void*)narrow_msg);
            
            char newline = '\n';
            UINTN nl_len = 1;
            uefi_call_wrapper(LogFile->Write, 3, LogFile, &nl_len, &newline);
            uefi_call_wrapper(LogFile->Flush, 1, LogFile);
        }
    }
    
    // 3. Write to Display Console
    if (wide_msg) {
        Print(L"%s\n\r", wide_msg);
    }
}

void Panic(const char* serial_msg, const CHAR16* screen_msg) {
    SystemLog("[FATAL] A critical boot error occurred.", L"[FATAL] A critical boot error occurred.");
    SystemLog(serial_msg, screen_msg);
    SystemLog("[*] System Halted.", L"[*] System Halted.");
    
    if (LogFile) uefi_call_wrapper(LogFile->Close, 1, LogFile);
    while (1) { __asm__ __volatile__("hlt"); }
}

EFI_GRAPHICS_OUTPUT_PROTOCOL* InitGOP() {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    if (!EFI_ERROR(uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop))) {
        SystemLog("[*] Graphics Output Protocol Initialized.", L"[*] Graphics Output Protocol Initialized.");
    }
    return gop;
}

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

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    InitSerial();
    SystemLog("=== OS Project UEFI Bootloader ===", L"=== OS Project UEFI Bootloader ===");
    
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    if (!Volume) {
        Panic("Could not mount EFI partition.", L"Could not mount EFI partition.");
        return EFI_LOAD_ERROR;
    }

    // Initialize Persistent File Logging
    uefi_call_wrapper(Volume->Open, 5, Volume, &LogFile, L"EFI\\BOOT\\boot.log", 
                      EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    if (LogFile) {
        SystemLog("[*] Persistent boot logging initialized.", L"[*] Persistent boot logging initialized.");
    }

    InitGOP();

    EFI_FILE_HANDLE KernelFile;
    if (EFI_ERROR(uefi_call_wrapper(Volume->Open, 5, Volume, &KernelFile, L"EFI\\BOOT\\bzImage", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY))) {
        Panic("Kernel (bzImage) not found.", L"Kernel (bzImage) not found.");
    } else {
        SystemLog("[*] Kernel file located.", L"[*] Kernel file located.");
        uefi_call_wrapper(KernelFile->Close, 1, KernelFile);
    }

    EFI_FILE_HANDLE ConfigFile;
    if (!EFI_ERROR(uefi_call_wrapper(Volume->Open, 5, Volume, &ConfigFile, L"EFI\\BOOT\\boot.cfg", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY))) {
        UINTN BufferSize = 512;
        CHAR8 Buffer[512];
        uefi_call_wrapper(ConfigFile->Read, 3, ConfigFile, &BufferSize, Buffer);
        Buffer[BufferSize] = '\0';
        SystemLog("[+] Command Line Extracted.", L"[+] Command Line Extracted.");
        uefi_call_wrapper(ConfigFile->Close, 1, ConfigFile);
    }

    SystemLog("[*] Halting CPU (End of Phase 1)...", L"[*] Halting CPU (End of Phase 1)...");
    
    if (LogFile) uefi_call_wrapper(LogFile->Close, 1, LogFile);
    while (1) { __asm__ __volatile__("hlt"); }
    return EFI_SUCCESS;
}
