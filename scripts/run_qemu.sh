#!/usr/bin/env bash
set -euo pipefail

# OS Project QEMU Execution Wrapper

MODE="${1:-headless}"
IMAGE="build/disk.img"
OVMF="/usr/share/OVMF/OVMF_CODE.fd" # Default Linux path, adjust for Windows/WSL if needed
LOG_FILE="build/qemu-serial.log"

echo "=== OS Project QEMU Environment ==="

if [ ! -f "$OVMF" ]; then
    echo "[!] Warning: OVMF firmware not found at $OVMF. UEFI boot may fail."
fi

# Basic resource limits defined in ARCH-011
QEMU_CMD=(
    qemu-system-x86_64
    -m 2G
    -smp 2
    -drive if=pflash,format=raw,readonly=on,file="$OVMF"
)

if [ -f "$IMAGE" ]; then
    QEMU_CMD+=(-drive file="$IMAGE",format=raw)
else
    echo "[!] Warning: $IMAGE not found. QEMU will boot without a disk."
fi

case "$MODE" in
    headless)
        echo "[*] Launching QEMU in headless mode (serial logged to $LOG_FILE)..."
        "${QEMU_CMD[@]}" -nographic -serial file:"$LOG_FILE"
        ;;
    gui)
        echo "[*] Launching QEMU with VGA output..."
        "${QEMU_CMD[@]}" -vga virtio -serial stdio
        ;;
    *)
        echo "Usage: $0 {headless|gui}"
        exit 1
        ;;
esac
