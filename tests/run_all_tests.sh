#!/bin/bash
set -e

echo "============================================================"
echo "          NEXUS OS - GLOBAL TEST ORCHESTRATOR               "
echo "============================================================"

# Define directories
BUILD_DIR="../build/tests"
mkdir -p "$BUILD_DIR"

echo "[*] Compiling Unit Tests..."
g++ -Wall -Wextra -std=c++17 unit/framework_test.cpp -o "$BUILD_DIR/framework_test"

echo ""
echo "[*] Running Unit Tests..."
"$BUILD_DIR/framework_test"
UNIT_EXIT=$?

echo ""
echo "[*] Running Smoke Tests..."
cd smoke
./boot_test.sh
SMOKE_EXIT=$?
cd ..

echo ""
echo "============================================================"
if [ $UNIT_EXIT -eq 0 ] && [ $SMOKE_EXIT -eq 0 ]; then
    echo "STATUS: ALL TEST SUITES PASSED SUCCESSFULLY."
    exit 0
else
    echo "STATUS: ONE OR MORE TEST SUITES FAILED."
    exit 1
fi
