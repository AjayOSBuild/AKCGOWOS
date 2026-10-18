#include <efi.h>
#include <efilib.h>

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
    
    Print(L"=== OS Project UEFI Bootloader ===\n\r");
    
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    if (!Volume) {
        Print(L"[!] Error: Could not mount EFI System Partition.\n\r");
        return EFI_LOAD_ERROR;
    }

    // 1. Locate and load the Kernel (BOOT-004)
    EFI_FILE_HANDLE KernelFile;
    EFI_STATUS Status = uefi_call_wrapper(Volume->Open, 5, Volume, &KernelFile, L"EFI\\BOOT\\bzImage", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        Print(L"[!] Error: Kernel (bzImage) not found.\n\r");
    } else {
        Print(L"[*] Kernel file located.\n\r");
        uefi_call_wrapper(KernelFile->Close, 1, KernelFile);
    }

    // 2. Parse the Command Line (BOOT-006)
    EFI_FILE_HANDLE ConfigFile;
    Status = uefi_call_wrapper(Volume->Open, 5, Volume, &ConfigFile, L"EFI\\BOOT\\boot.cfg", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    
    if (EFI_ERROR(Status)) {
        Print(L"[!] Error: boot.cfg not found. Using default arguments.\n\r");
    } else {
        Print(L"[*] boot.cfg opened. Parsing command line...\n\r");
        
        // Allocate buffer and read file (simplified for prototyping)
        UINTN BufferSize = 512;
        CHAR8 Buffer[512];
        uefi_call_wrapper(ConfigFile->Read, 3, ConfigFile, &BufferSize, Buffer);
        Buffer[BufferSize] = '\0'; // Null terminate
        
        // Basic string search for "cmdline="
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
            // Replace newline with null terminator to isolate the string
            for (UINTN i = 0; cmd_start[i] != '\0'; i++) {
                if (cmd_start[i] == '\n' || cmd_start[i] == '\r') {
                    cmd_start[i] = '\0';
                    break;
                }
            }
            // Print the extracted narrow string (CHAR8) to UEFI console
            Print(L"[+] Command Line Extracted: %a\n\r", cmd_start);
        }
        
        uefi_call_wrapper(ConfigFile->Close, 1, ConfigFile);
    }

    Print(L"[*] Halting CPU...\n\r");
    while (1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
