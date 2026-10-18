#!/usr/bin/env bash
set -euo pipefail

MODE="${1:-headless}"
IMAGE="build/disk.img"
OVMF="/usr/share/OVMF/OVMF_CODE.fd"
LOG_FILE="build/qemu-serial.log"

echo "=== OS Project QEMU Environment ==="

if [ ! -f "$OVMF" ]; then
    echo "[!] Warning: OVMF firmware not found at $OVMF. UEFI boot may fail."
fi

QEMU_CMD=(
    qemu-system-x86_64
    -m 2G
    -smp 2
    -drive if=pflash,format=raw,readonly=on,file="$OVMF"
)

if [ -f "$IMAGE" ]; then
    QEMU_CMD+=(-drive file="$IMAGE",format=raw)
fi

# Add full hardware topology for testing Phase 3 driver initialization
HW_FLAGS=(
    -device qemu-xhci,id=xhci
    -device usb-tablet,bus=xhci.0
    -device usb-kbd,bus=xhci.0
    -device intel-hda -device hda-output
    -netdev user,id=net0 -device e1000,netdev=net0
)

case "$MODE" in
    headless)
        echo "[*] Launching QEMU in headless mode..."
        "${QEMU_CMD[@]}" -nographic -serial file:"$LOG_FILE"
        ;;
    gui)
        echo "[*] Launching QEMU with VGA output..."
        "${QEMU_CMD[@]}" -vga virtio -serial stdio
        ;;
    full-hw)
        echo "[*] Launching QEMU with full Phase 3 hardware topology..."
        "${QEMU_CMD[@]}" "${HW_FLAGS[@]}" -vga virtio -serial file:"$LOG_FILE"
        ;;
    *)
        echo "Usage: $0 {headless|gui|full-hw}"
        exit 1
        ;;
esac
