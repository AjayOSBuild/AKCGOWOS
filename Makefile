# OS Project Root Makefile

.PHONY: all clean format boot kernel user iso

BUILD_DIR := build
SCRIPT_DIR := scripts

all: boot kernel user iso

boot:
	@echo "Building UEFI bootloader..."
	# $(MAKE) -C boot

kernel:
	@echo "Building kernel..."
	$(MAKE) -C kernel

user:
	@echo "Building user space..."
	# $(MAKE) -C core

iso:
	@echo "Generating bootable image..."
	# ./scripts/gen_iso.sh

format:
	@echo "Formatting C/C++ files..."
	find . -type f -name '*.[ch]' -o -name '*.[ch]pp' | xargs clang-format -i

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)/*
	# $(MAKE) -C boot clean
	$(MAKE) -C kernel clean
