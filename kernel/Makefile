# Kernel Subsystem Makefile

BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/bzImage
CONFIG_FILE := minimal_defconfig

.PHONY: all clean fetch config build_dummy

all: build_dummy

# Placeholder target for Phase 2 early integration testing.
# In KERN-002, this will be replaced with actual kernel compilation.
build_dummy:
	@echo "[*] (KERN-001) Generating dummy kernel bzImage for bootloader integration..."
	@mkdir -p $(BUILD_DIR)
	@echo "DUMMY_LINUX_KERNEL_PAYLOAD" > $(TARGET)
	@echo "[+] Dummy bzImage ready at $(TARGET)"

# Future hooks for KERN-002
fetch:
	@echo "Downloading Linux kernel source..."

config:
	@echo "Applying $(CONFIG_FILE)..."

clean:
	@rm -f $(TARGET)
