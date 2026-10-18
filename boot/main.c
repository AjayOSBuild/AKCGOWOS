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
    outb(COM1_PORT + 1, 0x00);    // Disable all interrupts
    outb(COM1_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_PORT + 1, 0x00);    //                  (hi byte)
    outb(COM1_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void SerialWrite(const char* str) {
    while (*str) {
        while ((inb(COM1_PORT + 5) & 0x20) == 0); // Wait for transmit buffer to be empty
        outb(COM1_PORT, *str++);
    }
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

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    // Initialize headless logging
    InitSerial();
    SerialWrite("\n\n=== OS Project UEFI Bootloader ===\n");
    SerialWrite("[*] COM1 Serial Console Initialized.\n");
    Print(L"=== OS Project UEFI Bootloader ===\n\r");
    Print(L"[*] Serial console initialized on COM1.\n\r");
    
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    if (!Volume) {
        SerialWrite("[!] Error: Could not mount EFI System Partition.\n");
        Print(L"[!] Error: Could not mount EFI System Partition.\n\r");
        return EFI_LOAD_ERROR;
    }

    // 1. Locate the Kernel
    EFI_FILE_HANDLE KernelFile;
    EFI_STATUS Status = uefi_call_wrapper(Volume->Open, 5, Volume, &KernelFile, L"EFI\\BOOT\\bzImage", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        SerialWrite("[!] Error: Kernel (bzImage) not found.\n");
        Print(L"[!] Error: Kernel (bzImage) not found.\n\r");
    } else {
        SerialWrite("[*] Kernel file located.\n");
        Print(L"[*] Kernel file located.\n\r");
        uefi_call_wrapper(KernelFile->Close, 1, KernelFile);
    }

    // 2. Parse the Command Line
    EFI_FILE_HANDLE ConfigFile;
    Status = uefi_call_wrapper(Volume->Open, 5, Volume, &ConfigFile, L"EFI\\BOOT\\boot.cfg", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    
    if (!EFI_ERROR(Status)) {
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
                if (cmd_start[i] == '\n' || cmd_start[i] == '\r') {
                    cmd_start[i] = '\0';
                    break;
                }
            }
            SerialWrite("[+] Command Line Extracted: ");
            SerialWrite((const char*)cmd_start);
            SerialWrite("\n");
            Print(L"[+] Command Line Extracted: %a\n\r", cmd_start);
        }
        uefi_call_wrapper(ConfigFile->Close, 1, ConfigFile);
    }

    SerialWrite("[*] Halting CPU...\n");
    Print(L"[*] Halting CPU...\n\r");
    while (1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
