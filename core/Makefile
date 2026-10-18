# Core Subsystem Makefile

BUILD_DIR := ../build
INIT_TARGET := $(BUILD_DIR)/init
SM_TARGET := $(BUILD_DIR)/service_manager
DM_TARGET := $(BUILD_DIR)/device_manager
SESS_TARGET := $(BUILD_DIR)/session_manager
UM_TARGET := $(BUILD_DIR)/user_manager

CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -static -O2 -pthread
CXXFLAGS := -Wall -Wextra -static -O2 -std=c++17

.PHONY: all clean

all: $(INIT_TARGET) $(SM_TARGET) $(DM_TARGET) $(SESS_TARGET) $(UM_TARGET)

$(INIT_TARGET): init.c
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling core init daemon..."
	$(CC) $(CFLAGS) -o $(INIT_TARGET) init.c

$(SM_TARGET): service_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Service Manager..."
	$(CXX) $(CXXFLAGS) -o $(SM_TARGET) service_manager.cpp

$(DM_TARGET): device_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Device Manager..."
	$(CXX) $(CXXFLAGS) -o $(DM_TARGET) device_manager.cpp

$(SESS_TARGET): session_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Session Manager..."
	$(CXX) $(CXXFLAGS) -o $(SESS_TARGET) session_manager.cpp

$(UM_TARGET): user_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ User Manager..."
	$(CXX) $(CXXFLAGS) -o $(UM_TARGET) user_manager.cpp

clean:
	@rm -f $(INIT_TARGET) $(SM_TARGET) $(DM_TARGET) $(SESS_TARGET) $(UM_TARGET)
