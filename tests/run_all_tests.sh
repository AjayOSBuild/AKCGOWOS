#!/bin/bash
set -e

echo "============================================================"
echo "          NEXUS OS - GLOBAL TEST ORCHESTRATOR               "
echo "============================================================"
echo "[*] Initializing QA Pipeline..."

BUILD_DIR="../build/tests"
mkdir -p "$BUILD_DIR"

echo "[*] Compiling C++ Unit Tests..."
g++ -Wall -Wextra -std=c++17 unit/framework_test.cpp -o "$BUILD_DIR/framework_test"
g++ -Wall -Wextra -std=c++17 unit/filesystem_test.cpp -o "$BUILD_DIR/filesystem_test"
g++ -Wall -Wextra -std=c++17 unit/networking_test.cpp -o "$BUILD_DIR/networking_test"
g++ -Wall -Wextra -std=c++17 unit/graphics_test.cpp -o "$BUILD_DIR/graphics_test"
g++ -Wall -Wextra -std=c++17 unit/wincompat_test.cpp -o "$BUILD_DIR/wincompat_test"
g++ -Wall -Wextra -std=c++17 unit/android_test.cpp -o "$BUILD_DIR/android_test"
g++ -Wall -Wextra -std=c++17 unit/security_test.cpp -o "$BUILD_DIR/security_test"
g++ -Wall -Wextra -std=c++17 unit/package_manager_test.cpp -o "$BUILD_DIR/package_manager_test"

echo ""
echo "------------------------------------------------------------"
echo "                      UNIT TESTS                            "
echo "------------------------------------------------------------"
"$BUILD_DIR/framework_test"
"$BUILD_DIR/filesystem_test"
"$BUILD_DIR/networking_test"
"$BUILD_DIR/graphics_test"
"$BUILD_DIR/wincompat_test"
"$BUILD_DIR/android_test"
"$BUILD_DIR/security_test"
"$BUILD_DIR/package_manager_test"
UNIT_EXIT=$?

echo ""
echo "------------------------------------------------------------"
echo "                   INTEGRATION TESTS                        "
echo "------------------------------------------------------------"
cd integration
./fs_integration_test.sh
./net_integration_test.sh
./gfx_integration_test.sh
./wincompat_integration_test.sh
./android_integration_test.sh
./sec_integration_test.sh
./pkg_integration_test.sh
INT_EXIT=$?
cd ..

echo ""
echo "------------------------------------------------------------"
echo "                      SMOKE TESTS                           "
echo "------------------------------------------------------------"
cd smoke
./boot_test.sh
SMOKE_EXIT=$?
cd ..

echo ""
echo "============================================================"
echo "                   FINAL QA REPORT                          "
echo "============================================================"
if [ $UNIT_EXIT -eq 0 ] && [ $INT_EXIT -eq 0 ] && [ $SMOKE_EXIT -eq 0 ]; then
    echo "[+] SYSTEM STATUS : STABLE"
    echo "[+] ALL QA PIPELINES PASSED SUCCESSFULLY."
    exit 0
else
    echo "[!] SYSTEM STATUS : UNSTABLE"
    echo "[!] ONE OR MORE QA PIPELINES FAILED."
    exit 1
fi
