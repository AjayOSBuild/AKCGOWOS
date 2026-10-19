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
SET_BIN="$BUILD_DIR/settings"
CALC_BIN="$BUILD_DIR/calculator"
TE_BIN="$BUILD_DIR/text_editor"
MP_BIN="$BUILD_DIR/media_player"
NM_BIN="$BUILD_DIR/network_manager"
UPM_BIN="$BUILD_DIR/update_manager"
NS_BIN="$BUILD_DIR/notification_service"
AM_BIN="$BUILD_DIR/application_manager"
PRM_BIN="$BUILD_DIR/process_manager"
SES_BIN="$BUILD_DIR/system_event_service"
COMP_BIN="$BUILD_DIR/compositor"
TERM_BIN="$BUILD_DIR/terminal"
TM_BIN="$BUILD_DIR/task_manager"
SET_BIN="$BUILD_DIR/settings"
CALC_BIN="$BUILD_DIR/calculator"
TE_BIN="$BUILD_DIR/text_editor"
MP_BIN="$BUILD_DIR/media_player"
DS_BIN="$BUILD_DIR/desktop_shell"

echo "[*] Generating initial RAM filesystem (initramfs)..."

if [ ! -f "$INIT_BIN" ] || [ ! -f "$SM_BIN" ] || [ ! -f "$DM_BIN" ] || [ ! -f "$SESS_BIN" ] || [ ! -f "$UM_BIN" ] || [ ! -f "$PM_BIN" ] || [ ! -f "$CS_BIN" ] || [ ! -f "$LS_BIN" ] || [ ! -f "$STM_BIN" ] || [ ! -f "$NM_BIN" ] || [ ! -f "$UPM_BIN" ] || [ ! -f "$NS_BIN" ] || [ ! -f "$AM_BIN" ] || [ ! -f "$PRM_BIN" ] || [ ! -f "$SES_BIN" ] || [ ! -f "$COMP_BIN" ] || [ ! -f "$TERM_BIN" ] || [ ! -f "$TM_BIN" ] || [ ! -f "$SET_BIN" ] || [ ! -f "$CALC_BIN" ] || [ ! -f "$TE_BIN" ] || [ ! -f "$MP_BIN" ] || [ ! -f "$DS_BIN" ]; then
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
cp "../../$UPM_BIN" ./sbin/update_manager
cp "../../$NS_BIN" ./sbin/notification_service
cp "../../$AM_BIN" ./sbin/application_manager
cp "../../$PRM_BIN" ./sbin/process_manager
cp "../../$SES_BIN" ./sbin/system_event_service
cp "../../$COMP_BIN" ./sbin/compositor ./bin/terminal ./bin/task_manager ./bin/settings ./bin/calculator ./bin/text_editor ./bin/media_player
cp "../../$DS_BIN" ./sbin/desktop_shell
chmod +x ./init ./sbin/service_manager ./sbin/device_manager ./sbin/session_manager ./sbin/user_manager ./sbin/permission_manager ./sbin/configuration_service ./sbin/logging_service ./sbin/storage_manager ./sbin/network_manager ./sbin/update_manager ./sbin/notification_service ./sbin/application_manager ./sbin/process_manager ./sbin/system_event_service ./sbin/compositor ./bin/terminal ./bin/task_manager ./bin/settings ./bin/calculator ./bin/text_editor ./bin/media_player ./sbin/desktop_shell

# Package into cpio archive and gzip
find . -print0 | cpio --null -ov --format=newc 2>/dev/null | gzip -9 > "../initramfs.cpio.gz"

cd ../..
echo "[+] Initramfs generated at $OUTPUT"
