# Core Subsystem Makefile

BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/init

CC := gcc
# Compile statically so it doesn't depend on shared libraries in the minimal initramfs
CFLAGS := -Wall -Wextra -static -O2

.PHONY: all clean

all: $(TARGET)

$(TARGET): init.c
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling core init daemon..."
	$(CC) $(CFLAGS) -o $(TARGET) init.c

clean:
	@rm -f $(TARGET)
