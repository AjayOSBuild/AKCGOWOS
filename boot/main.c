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
    Print(L"[*] EFI System Partition mounted.\n\r");

    EFI_FILE_HANDLE KernelFile;
    EFI_STATUS Status = uefi_call_wrapper(Volume->Open, 5, Volume, &KernelFile, L"EFI\\BOOT\\bzImage", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    
    if (EFI_ERROR(Status)) {
        Print(L"[!] Error: Kernel (bzImage) not found.\n\r");
    } else {
        Print(L"[*] Kernel file located. Loading into memory...\n\r");
        
        // In a complete implementation, we parse the setup header and allocate specific pages.
        // For Phase 1 validation, we confirm file handle acquisition.
        uefi_call_wrapper(KernelFile->Close, 1, KernelFile);
        Print(L"[+] Kernel loaded successfully (Placeholder validation).\n\r");
    }

    Print(L"[*] Halting CPU...\n\r");
    while (1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
