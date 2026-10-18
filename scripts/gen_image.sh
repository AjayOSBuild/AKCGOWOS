#!/usr/bin/env bash
set -euo pipefail

IMAGE="build/disk.img"
BOOT_EFI="boot/bootx64.efi"
BOOT_CFG="boot/boot.cfg"
KERNEL_DUMMY="build/bzImage"

echo "[*] Generating UEFI bootable disk image..."

if [ ! -f "$BOOT_EFI" ]; then
    echo "[!] Error: $BOOT_EFI not found. Run 'make boot' first."
    exit 1
fi

mkdir -p build

# Create a dummy kernel file for bootloader validation
echo "DUMMY_KERNEL_PAYLOAD" > "$KERNEL_DUMMY"

# Create a 20MB empty image
dd if=/dev/zero of="$IMAGE" bs=1M count=20 status=none

# Format as FAT32
mkfs.fat -F 32 "$IMAGE"

# Create EFI directory structure and copy files
mmd -i "$IMAGE" ::/EFI
mmd -i "$IMAGE" ::/EFI/BOOT
mcopy -i "$IMAGE" "$BOOT_EFI" ::/EFI/BOOT/BOOTX64.EFI
mcopy -i "$IMAGE" "$BOOT_CFG" ::/EFI/BOOT/boot.cfg
mcopy -i "$IMAGE" "$KERNEL_DUMMY" ::/EFI/BOOT/bzImage

echo "[+] Disk image generated at $IMAGE"
