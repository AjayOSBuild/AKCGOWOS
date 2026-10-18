#!/usr/bin/env bash
set -euo pipefail

LOG_FILE="build/qemu-serial.log"
TIMEOUT=4

echo "=== OS Project Automated Failure Test (Missing Kernel) ==="

# Build OS image with SIMULATE_MISSING_KERNEL flag
export SIMULATE_MISSING_KERNEL=1
./scripts/build.sh all > /dev/null

rm -f "$LOG_FILE"
touch "$LOG_FILE"

echo "[*] Starting QEMU (Timeout: ${TIMEOUT}s)..."
./scripts/run_qemu.sh headless > /dev/null 2>&1 &
QEMU_PID=$!
trap 'kill $QEMU_PID 2>/dev/null || true' EXIT

sleep "$TIMEOUT"

echo "[*] Analyzing QEMU serial logs for failure detection..."

EXPECTED_FATAL="\[FATAL\] Kernel (bzImage) not found"

if grep -q "$EXPECTED_FATAL" "$LOG_FILE"; then
    echo "  [PASS] Bootloader successfully trapped and logged the missing kernel."
    exit 0
else
    echo "  [FAIL] Bootloader did not trap the missing kernel error."
    cat "$LOG_FILE"
    exit 1
fi
