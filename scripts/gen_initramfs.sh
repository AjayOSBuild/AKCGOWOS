#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
INITRAMFS_DIR="$BUILD_DIR/initramfs_staging"
OUTPUT="$BUILD_DIR/initramfs.cpio.gz"
INIT_BIN="$BUILD_DIR/init"
SM_BIN="$BUILD_DIR/service_manager"
DM_BIN="$BUILD_DIR/device_manager"
SESS_BIN="$BUILD_DIR/session_manager"
UM_BIN="$BUILD_DIR/user_manager"
PM_BIN="$BUILD_DIR/permission_manager"
CS_BIN="$BUILD_DIR/configuration_service"
LS_BIN="$BUILD_DIR/logging_service"
STM_BIN="$BUILD_DIR/storage_manager"
NM_BIN="$BUILD_DIR/network_manager"

echo "[*] Generating initial RAM filesystem (initramfs)..."

if [ ! -f "$INIT_BIN" ] || [ ! -f "$SM_BIN" ] || [ ! -f "$DM_BIN" ] || [ ! -f "$SESS_BIN" ] || [ ! -f "$UM_BIN" ] || [ ! -f "$PM_BIN" ] || [ ! -f "$CS_BIN" ] || [ ! -f "$LS_BIN" ] || [ ! -f "$STM_BIN" ] || [ ! -f "$NM_BIN" ]; then
    echo "[!] Error: Core binaries not found. Run 'make user' first."
    exit 1
fi

rm -rf "$INITRAMFS_DIR"
mkdir -p "$INITRAMFS_DIR"
cd "$INITRAMFS_DIR"

# Scaffold directories
mkdir -p bin sbin etc/os proc sys dev tmp var/log/os mnt root home/default_user/.config/os

# Copy the statically compiled core binaries
cp "../../$INIT_BIN" ./init
cp "../../$SM_BIN" ./sbin/service_manager
cp "../../$DM_BIN" ./sbin/device_manager
cp "../../$SESS_BIN" ./sbin/session_manager
cp "../../$UM_BIN" ./sbin/user_manager
cp "../../$PM_BIN" ./sbin/permission_manager
cp "../../$CS_BIN" ./sbin/configuration_service
cp "../../$LS_BIN" ./sbin/logging_service
cp "../../$STM_BIN" ./sbin/storage_manager
cp "../../$NM_BIN" ./sbin/network_manager
chmod +x ./init ./sbin/service_manager ./sbin/device_manager ./sbin/session_manager ./sbin/user_manager ./sbin/permission_manager ./sbin/configuration_service ./sbin/logging_service ./sbin/storage_manager ./sbin/network_manager

# Package into cpio archive and gzip
find . -print0 | cpio --null -ov --format=newc 2>/dev/null | gzip -9 > "../initramfs.cpio.gz"

cd ../..
echo "[+] Initramfs generated at $OUTPUT"
