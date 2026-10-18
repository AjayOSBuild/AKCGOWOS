# Core Subsystem Makefile

BUILD_DIR := ../build
INIT_TARGET := $(BUILD_DIR)/init
SM_TARGET := $(BUILD_DIR)/service_manager

CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -static -O2 -pthread
CXXFLAGS := -Wall -Wextra -static -O2 -std=c++17

.PHONY: all clean

all: $(INIT_TARGET) $(SM_TARGET)

$(INIT_TARGET): init.c
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling core init daemon..."
	$(CC) $(CFLAGS) -o $(INIT_TARGET) init.c

$(SM_TARGET): service_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Service Manager..."
	$(CXX) $(CXXFLAGS) -o $(SM_TARGET) service_manager.cpp

clean:
	@rm -f $(INIT_TARGET) $(SM_TARGET)
