#!/bin/bash
set -e

ISO_PATH="build/nexus-os-installer.iso"
OVMF_PATH="/usr/share/OVMF/OVMF_CODE.fd" # Standard path on Debian/Ubuntu

echo "[*] Nexus OS UEFI Boot Tester"

if [ ! -f "$ISO_PATH" ]; then
    echo "[!] Error: ISO not found at $ISO_PATH. Run scripts/build_iso.sh first."
    exit 1
fi

if [ ! -f "$OVMF_PATH" ]; then
    # Fallback for Arch/Fedora paths
    if [ -f "/usr/share/ovmf/x64/OVMF_CODE.fd" ]; then
        OVMF_PATH="/usr/share/ovmf/x64/OVMF_CODE.fd"
    else
        echo "[!] Warning: OVMF firmware not found at standard paths."
        echo "    Please install 'ovmf' or 'edk2-ovmf' package to test UEFI."
        echo "    Attempting to run with default BIOS..."
        qemu-system-x86_64 -cdrom "$ISO_PATH" -m 1024 -serial stdio -display none
        exit 0
    fi
fi

echo "[*] Launching QEMU with UEFI Firmware (OVMF)..."
qemu-system-x86_64 \
    -drive if=pflash,format=raw,readonly=on,file=$OVMF_PATH \
    -cdrom "$ISO_PATH" \
    -m 1024 \
    -serial stdio \
    -display none
