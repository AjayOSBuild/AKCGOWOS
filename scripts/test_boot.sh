#!/usr/bin/env bash
set -euo pipefail

LOG_FILE="build/qemu-serial.log"
TIMEOUT=5

echo "=== OS Project Automated Boot Test ==="

# 1. Build the latest image
echo "[*] Building OS image..."
./scripts/build.sh all > /dev/null

# 2. Clear old logs
rm -f "$LOG_FILE"
touch "$LOG_FILE"

# 3. Start QEMU headlessly in the background
echo "[*] Starting QEMU (Timeout: ${TIMEOUT}s)..."
./scripts/run_qemu.sh headless > /dev/null 2>&1 &
QEMU_PID=$!

# Ensure QEMU is killed when the script exits
trap 'kill $QEMU_PID 2>/dev/null || true' EXIT

# 4. Wait for boot sequence to complete
sleep "$TIMEOUT"

# 5. Analyze the log
echo "[*] Analyzing QEMU serial logs..."

# Array of expected strings from BOOT-004 through BOOT-009
EXPECTED=(
    "COM1 Serial Console Initialized"
    "Memory map query successful"
    "Kernel file located"
    "Command Line Extracted"
    "Halting CPU"
)

FAILED=0

for STR in "${EXPECTED[@]}"; do
    if grep -q "$STR" "$LOG_FILE"; then
        echo "  [PASS] Found: '$STR'"
    else
        echo "  [FAIL] Missing: '$STR'"
        FAILED=1
    fi
done

if [ "$FAILED" -eq 1 ]; then
    echo "[!] Boot Test FAILED. Dumping log:"
    cat "$LOG_FILE"
    exit 1
else
    echo "[+] Boot Test PASSED. All subsystems initialized correctly."
    exit 0
fi
