CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

VM_QEMU_TARGET = $(BUILD_DIR)/qemu_wrapper
VM_QEMU_SRCS = qemu_wrapper.cpp

VM_MGR_TARGET = $(BUILD_DIR)/vm_manager
VM_MGR_SRCS = vm_manager.cpp qemu_wrapper.cpp virtual_disk.cpp cgroup_manager.cpp

all: $(VM_QEMU_TARGET) $(VM_MGR_TARGET)

$(VM_QEMU_TARGET): $(VM_QEMU_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ QEMU Wrapper..."
	$(CXX) $(CXXFLAGS) -o $(VM_QEMU_TARGET) $(VM_QEMU_SRCS)

$(VM_MGR_TARGET): $(VM_MGR_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ VM Manager with Cgroup Resource Limits..."
	$(CXX) $(CXXFLAGS) -o $(VM_MGR_TARGET) $(VM_MGR_SRCS)

clean:
	rm -f $(VM_QEMU_TARGET) $(VM_MGR_TARGET)
