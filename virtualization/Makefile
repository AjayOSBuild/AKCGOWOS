CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

VM_QEMU_TARGET = $(BUILD_DIR)/qemu_wrapper
VM_QEMU_SRCS = qemu_wrapper.cpp

VM_MGR_TARGET = $(BUILD_DIR)/vm_manager
VM_MGR_SRCS = vm_manager.cpp qemu_wrapper.cpp virtual_disk.cpp cgroup_manager.cpp

VM_SANDBOX_TARGET = $(BUILD_DIR)/app_sandbox
VM_SANDBOX_SRCS = app_sandbox.cpp vm_manager.cpp qemu_wrapper.cpp virtual_disk.cpp cgroup_manager.cpp

VM_TEST_TARGET = $(BUILD_DIR)/vm_test_env
VM_TEST_SRCS = vm_test_env.cpp vm_manager.cpp qemu_wrapper.cpp virtual_disk.cpp cgroup_manager.cpp

VM_UI_TARGET = $(BUILD_DIR)/vm_ui
VM_UI_SRCS = vm_ui.cpp vm_manager.cpp qemu_wrapper.cpp virtual_disk.cpp cgroup_manager.cpp

all: $(VM_QEMU_TARGET) $(VM_MGR_TARGET) $(VM_SANDBOX_TARGET) $(VM_TEST_TARGET) $(VM_UI_TARGET)

$(VM_QEMU_TARGET): $(VM_QEMU_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(VM_QEMU_TARGET) $(VM_QEMU_SRCS)

$(VM_MGR_TARGET): $(VM_MGR_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(VM_MGR_TARGET) $(VM_MGR_SRCS)

$(VM_SANDBOX_TARGET): $(VM_SANDBOX_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(VM_SANDBOX_TARGET) $(VM_SANDBOX_SRCS)

$(VM_TEST_TARGET): $(VM_TEST_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(VM_TEST_TARGET) $(VM_TEST_SRCS)

$(VM_UI_TARGET): $(VM_UI_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ VM Management UI..."
	$(CXX) $(CXXFLAGS) -o $(VM_UI_TARGET) $(VM_UI_SRCS)

clean:
	rm -f $(VM_QEMU_TARGET) $(VM_MGR_TARGET) $(VM_SANDBOX_TARGET) $(VM_TEST_TARGET) $(VM_UI_TARGET)
