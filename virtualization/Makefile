CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

VM_QEMU_TARGET = $(BUILD_DIR)/qemu_wrapper
VM_QEMU_SRCS = qemu_wrapper.cpp

all: $(VM_QEMU_TARGET)

$(VM_QEMU_TARGET): $(VM_QEMU_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ QEMU Wrapper..."
	$(CXX) $(CXXFLAGS) -o $(VM_QEMU_TARGET) $(VM_QEMU_SRCS)

clean:
	rm -f $(VM_QEMU_TARGET)
