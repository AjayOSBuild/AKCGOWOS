#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
INITRAMFS_DIR="$BUILD_DIR/initramfs_staging"
OUTPUT="$BUILD_DIR/initramfs.cpio.gz"

echo "[*] Generating initial RAM filesystem (initramfs)..."

# Clean previous staging area
rm -rf "$INITRAMFS_DIR"
mkdir -p "$INITRAMFS_DIR"
cd "$INITRAMFS_DIR"

# Scaffold basic Linux directory structure
mkdir -p bin sbin etc proc sys dev tmp var mnt root

# Create a minimal /init script
cat << 'INITEOF' > init
#!/bin/sh
echo "=== OS Project Initramfs Phase ==="
echo "[*] Mounting virtual filesystems..."
mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev

echo "[+] Initramfs successfully loaded. Handing over to Core Services..."
# Dummy loop for early testing without a real shell
while true; do sleep 1; done
INITEOF

chmod +x init

# Package into cpio archive and gzip
find . -print0 | cpio --null -ov --format=newc | gzip -9 > "../initramfs.cpio.gz"

cd ../..
echo "[+] Initramfs generated at $OUTPUT"
