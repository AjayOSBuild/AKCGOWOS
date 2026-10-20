#!/bin/bash
set -e

echo "[*] Nexus OS ISO Generator"

STAGING_DIR="installer/iso_staging"
OUTPUT_ISO="build/nexus-os-installer.iso"

mkdir -p build
mkdir -p $STAGING_DIR/boot/grub

# For development testing, mock the kernel and initramfs if they haven't been copied
if [ ! -f "$STAGING_DIR/boot/vmlinuz" ]; then
    echo "  -> Mocking vmlinuz for ISO generation test..."
    echo "mock_kernel" > $STAGING_DIR/boot/vmlinuz
fi

if [ ! -f "$STAGING_DIR/boot/initramfs.img" ]; then
    echo "  -> Mocking initramfs for ISO generation test..."
    echo "mock_initramfs" > $STAGING_DIR/boot/initramfs.img
fi

echo "[*] Building hybrid UEFI/BIOS ISO image..."
if command -v grub-mkrescue >/dev/null 2>&1; then
    grub-mkrescue -o $OUTPUT_ISO $STAGING_DIR -- -volid "NEXUS_OS" > /dev/null 2>&1
    echo "[+] ISO successfully generated: $OUTPUT_ISO"
    ls -lh $OUTPUT_ISO
else
    echo "[!] Error: grub-mkrescue not found."
    echo "    Please install xorriso, mtools, and grub-pc-bin/grub-efi-amd64-bin."
    exit 1
fi
