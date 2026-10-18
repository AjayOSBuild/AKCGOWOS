# Core Subsystem Makefile

BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/init

CC := gcc
# Compile statically and include pthread support
CFLAGS := -Wall -Wextra -static -O2 -pthread

.PHONY: all clean

all: $(TARGET)

$(TARGET): init.c
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling core init daemon with thread support..."
	$(CC) $(CFLAGS) -o $(TARGET) init.c

clean:
	@rm -f $(TARGET)
