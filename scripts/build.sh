#!/usr/bin/env bash
set -euo pipefail

# OS Project Master Build Script

ACTION="${1:-all}"
BUILD_DIR="build"

echo "=== OS Project Build System ==="

if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

case "$ACTION" in
    all)
        echo "[*] Starting full build..."
        make all
        echo "[+] Build complete."
        ;;
    clean)
        echo "[*] Cleaning build directory..."
        make clean
        echo "[+] Clean complete."
        ;;
    *)
        echo "Usage: $0 {all|clean}"
        exit 1
        ;;
esac
