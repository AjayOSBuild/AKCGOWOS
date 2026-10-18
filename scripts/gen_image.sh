#!/usr/bin/env bash
set -euo pipefail

IMAGE="build/disk.img"
BOOT_EFI="boot/bootx64.efi"
BOOT_CFG="boot/boot.cfg"
KERNEL_DUMMY="build/bzImage"
INITRAMFS="build/initramfs.cpio.gz"

echo "[*] Generating UEFI bootable disk image..."

if [ ! -f "$BOOT_EFI" ]; then
    echo "[!] Error: $BOOT_EFI not found. Run 'make boot' first."
    exit 1
fi

if [ ! -f "$INITRAMFS" ]; then
    echo "[*] $INITRAMFS not found. Generating it now..."
    ./scripts/gen_initramfs.sh
fi

mkdir -p build
echo "DUMMY_KERNEL_PAYLOAD" > "$KERNEL_DUMMY"

dd if=/dev/zero of="$IMAGE" bs=1M count=20 status=none
mkfs.fat -F 32 "$IMAGE"

mmd -i "$IMAGE" ::/EFI
mmd -i "$IMAGE" ::/EFI/BOOT
mcopy -i "$IMAGE" "$BOOT_EFI" ::/EFI/BOOT/BOOTX64.EFI
mcopy -i "$IMAGE" "$BOOT_CFG" ::/EFI/BOOT/boot.cfg
mcopy -i "$IMAGE" "$KERNEL_DUMMY" ::/EFI/BOOT/bzImage
mcopy -i "$IMAGE" "$INITRAMFS" ::/EFI/BOOT/initramfs.cpio.gz

echo "[+] Disk image generated at $IMAGE"
