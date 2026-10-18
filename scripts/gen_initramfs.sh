#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
INITRAMFS_DIR="$BUILD_DIR/initramfs_staging"
OUTPUT="$BUILD_DIR/initramfs.cpio.gz"
INIT_BIN="$BUILD_DIR/init"
SM_BIN="$BUILD_DIR/service_manager"
DM_BIN="$BUILD_DIR/device_manager"

echo "[*] Generating initial RAM filesystem (initramfs)..."

if [ ! -f "$INIT_BIN" ] || [ ! -f "$SM_BIN" ] || [ ! -f "$DM_BIN" ]; then
    echo "[!] Error: Core binaries not found. Run 'make user' first."
    exit 1
fi

rm -rf "$INITRAMFS_DIR"
mkdir -p "$INITRAMFS_DIR"
cd "$INITRAMFS_DIR"

mkdir -p bin sbin etc proc sys dev tmp var mnt root

# Copy the statically compiled core binaries
cp "../../$INIT_BIN" ./init
cp "../../$SM_BIN" ./sbin/service_manager
cp "../../$DM_BIN" ./sbin/device_manager
chmod +x ./init ./sbin/service_manager ./sbin/device_manager

# Package into cpio archive and gzip
find . -print0 | cpio --null -ov --format=newc 2>/dev/null | gzip -9 > "../initramfs.cpio.gz"

cd ../..
echo "[+] Initramfs generated at $OUTPUT"
