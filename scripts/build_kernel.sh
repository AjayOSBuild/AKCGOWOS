#!/usr/bin/env bash
set -euo pipefail

# Kernel Build Configuration
KERNEL_VERSION="6.6.60" # Target LTS version
KERNEL_TAR="linux-${KERNEL_VERSION}.tar.xz"
KERNEL_URL="https://cdn.kernel.org/pub/linux/kernel/v6.x/${KERNEL_TAR}"
SRC_DIR="kernel/src/linux-${KERNEL_VERSION}"
TAR_PATH="kernel/src/${KERNEL_TAR}"
DEFCONFIG="kernel/minimal_defconfig"
OUT_DIR="build"
OUT_BZIMAGE="${OUT_DIR}/bzImage"

# Toggle for fast CI vs actual GCC compilation
REAL_BUILD="${REAL_KERNEL_BUILD:-0}"

echo "=== OS Project Kernel Build ==="

mkdir -p "$OUT_DIR"

if [ "$REAL_BUILD" -eq 0 ]; then
    echo "[!] REAL_KERNEL_BUILD=0: Generating dummy kernel for fast QEMU UI/Boot testing."
    echo "DUMMY_LINUX_KERNEL_PAYLOAD" > "$OUT_BZIMAGE"
    echo "[+] Dummy bzImage ready at $OUT_BZIMAGE"
    exit 0
fi

echo "[*] Preparing reproducible kernel build (v${KERNEL_VERSION})..."

mkdir -p kernel/src

# Fetch source if missing
if [ ! -f "$TAR_PATH" ]; then
    echo "[*] Downloading Linux kernel source..."
    wget -q -O "$TAR_PATH" "$KERNEL_URL"
fi

# Extract source if missing
if [ ! -d "$SRC_DIR" ]; then
    echo "[*] Extracting kernel source..."
    tar -xf "$TAR_PATH" -C kernel/src/
fi

# Configure and Build
echo "[*] Applying custom minimal_defconfig..."
cp "$DEFCONFIG" "${SRC_DIR}/arch/x86/configs/os_defconfig"

cd "$SRC_DIR"
make os_defconfig
make olddefconfig

echo "[*] Compiling bzImage..."
# Use available CPU cores for compilation speed
make -j$(nproc) bzImage

cd ../../..
cp "${SRC_DIR}/arch/x86/boot/bzImage" "$OUT_BZIMAGE"

echo "[+] Real bzImage compiled and copied to $OUT_BZIMAGE"
