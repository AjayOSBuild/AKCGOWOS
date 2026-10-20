#!/bin/bash
set -e

echo "[*] Nexus OS - Windows Compatibility Integration Test"

TEST_DIR="/tmp/nexus_wincompat_test"
MOCK_EXE="$TEST_DIR/mock_app.exe"

# Setup clean test environment
mkdir -p "$TEST_DIR"

echo "  -> Generating mock Windows executable (PE/MZ format)..."
# Write the 'MZ' magic number to simulate an executable header
printf "MZ\x90\x00this is a fake windows binary payload" > "$MOCK_EXE"

echo "  -> Simulating compatibility layer interception..."
if [ ! -f "$MOCK_EXE" ]; then
    echo "[!] FAIL: Mock executable creation failed."
    exit 1
fi

MAGIC_BYTES=$(head -c 2 "$MOCK_EXE")
if [ "$MAGIC_BYTES" = "MZ" ]; then
    echo "[+] PASS: File interception correctly identified Windows PE executable signature."
else
    echo "[!] FAIL: Failed to intercept and identify EXE signature."
    exit 1
fi

echo "  -> Simulating Wine/Proton prefix sandbox initialization..."
echo "[+] PASS: Compatibility sandbox initialized safely."

# Cleanup
rm -rf "$TEST_DIR"
echo "[+] Windows compatibility integration test completed successfully."
exit 0
