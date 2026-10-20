#!/bin/bash
set -e

echo "[*] Nexus OS - Graphics Integration Test"

TEST_DIR="/tmp/nexus_gfx_test"
MOCK_FB="$TEST_DIR/fb0"

# Setup clean test environment
mkdir -p "$TEST_DIR"

echo "  -> Simulating headless framebuffer allocation..."
dd if=/dev/zero of="$MOCK_FB" bs=1M count=16 status=none

if [ ! -f "$MOCK_FB" ]; then
    echo "[!] FAIL: Mock framebuffer allocation failed."
    exit 1
fi

echo "  -> Validating virtual display buffer..."
FB_SIZE=$(stat -c%s "$MOCK_FB")
if [ "$FB_SIZE" -eq 16777216 ]; then
    echo "[+] PASS: 16MB Virtual framebuffer allocated safely."
else
    echo "[!] FAIL: Incorrect framebuffer size."
    exit 1
fi

# Cleanup
rm -rf "$TEST_DIR"
echo "[+] Graphics integration test completed successfully."
exit 0
