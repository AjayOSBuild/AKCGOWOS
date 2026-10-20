#!/bin/bash
set -e

echo "============================================================"
echo "          NEXUS OS - GLOBAL TEST ORCHESTRATOR               "
echo "============================================================"

BUILD_DIR="../build/tests"
mkdir -p "$BUILD_DIR"

echo "[*] Compiling Unit Tests..."
g++ -Wall -Wextra -std=c++17 unit/framework_test.cpp -o "$BUILD_DIR/framework_test"
g++ -Wall -Wextra -std=c++17 unit/filesystem_test.cpp -o "$BUILD_DIR/filesystem_test"
g++ -Wall -Wextra -std=c++17 unit/networking_test.cpp -o "$BUILD_DIR/networking_test"

echo ""
echo "[*] Running Base Framework Tests..."
"$BUILD_DIR/framework_test"

echo ""
echo "[*] Running Unit Tests (Filesystem & Networking)..."
"$BUILD_DIR/filesystem_test"
"$BUILD_DIR/networking_test"
UNIT_EXIT=$?

echo ""
echo "[*] Running Integration Tests..."
cd integration
./fs_integration_test.sh
./net_integration_test.sh
INT_EXIT=$?
cd ..

echo ""
echo "[*] Running Smoke Tests..."
cd smoke
./boot_test.sh
SMOKE_EXIT=$?
cd ..

echo ""
echo "============================================================"
if [ $UNIT_EXIT -eq 0 ] && [ $INT_EXIT -eq 0 ] && [$SMOKE_EXIT -eq 0 ]; then
    echo "STATUS: ALL TEST SUITES PASSED SUCCESSFULLY."
    exit 0
else
    echo "STATUS: ONE OR MORE TEST SUITES FAILED."
    exit 1
fi
