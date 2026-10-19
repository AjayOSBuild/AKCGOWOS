# Core Subsystem Makefile

BUILD_DIR := ../build
INIT_TARGET := $(BUILD_DIR)/init
SM_TARGET := $(BUILD_DIR)/service_manager
DM_TARGET := $(BUILD_DIR)/device_manager
SESS_TARGET := $(BUILD_DIR)/session_manager
UM_TARGET := $(BUILD_DIR)/user_manager
PM_TARGET := $(BUILD_DIR)/permission_manager
CS_TARGET := $(BUILD_DIR)/configuration_service
LS_TARGET := $(BUILD_DIR)/logging_service
STM_TARGET := $(BUILD_DIR)/storage_manager
NM_TARGET := $(BUILD_DIR)/network_manager
UPM_TARGET := $(BUILD_DIR)/update_manager
NS_TARGET := $(BUILD_DIR)/notification_service
AM_TARGET := $(BUILD_DIR)/application_manager
PRM_TARGET := $(BUILD_DIR)/process_manager
SES_TARGET := $(BUILD_DIR)/system_event_service
COMP_TARGET := $(BUILD_DIR)/compositor
DS_TARGET := $(BUILD_DIR)/desktop_shell

CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -static -O2 -pthread
CXXFLAGS := -Wall -Wextra -static -O2 -std=c++17

.PHONY: all clean

all: $(INIT_TARGET) $(SM_TARGET) $(DM_TARGET) $(SESS_TARGET) $(UM_TARGET) $(PM_TARGET) $(CS_TARGET) $(LS_TARGET) $(STM_TARGET) $(NM_TARGET) $(UPM_TARGET) $(NS_TARGET) $(AM_TARGET) $(PRM_TARGET) $(SES_TARGET) $(COMP_TARGET) $(DS_TARGET)

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

$(PM_TARGET): permission_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Permission Manager..."
	$(CXX) $(CXXFLAGS) -o $(PM_TARGET) permission_manager.cpp

$(CS_TARGET): configuration_service.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Configuration Service..."
	$(CXX) $(CXXFLAGS) -o $(CS_TARGET) configuration_service.cpp

$(LS_TARGET): logging_service.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Logging Service..."
	$(CXX) $(CXXFLAGS) -o $(LS_TARGET) logging_service.cpp

$(STM_TARGET): storage_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Storage Manager..."
	$(CXX) $(CXXFLAGS) -o $(STM_TARGET) storage_manager.cpp

$(NM_TARGET): network_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Network Manager..."
	$(CXX) $(CXXFLAGS) -o $(NM_TARGET) network_manager.cpp

$(UPM_TARGET): update_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Update Manager..."
	$(CXX) $(CXXFLAGS) -o $(UPM_TARGET) update_manager.cpp

$(NS_TARGET): notification_service.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Notification Service..."
	$(CXX) $(CXXFLAGS) -o $(NS_TARGET) notification_service.cpp

$(AM_TARGET): application_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Application Manager..."
	$(CXX) $(CXXFLAGS) -o $(AM_TARGET) application_manager.cpp

$(PRM_TARGET): process_manager.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Process Manager..."
	$(CXX) $(CXXFLAGS) -o $(PRM_TARGET) process_manager.cpp

$(SES_TARGET): system_event_service.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ System Event Service..."
	$(CXX) $(CXXFLAGS) -o $(SES_TARGET) system_event_service.cpp

$(COMP_TARGET): compositor.cpp compositor.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Graphics Compositor..."
	$(CXX) $(CXXFLAGS) -o $(COMP_TARGET) compositor.cpp

$(DS_TARGET): desktop_shell.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Desktop Shell..."
	$(CXX) $(CXXFLAGS) -o $(DS_TARGET) desktop_shell.cpp

clean:
	@rm -f $(INIT_TARGET) $(SM_TARGET) $(DM_TARGET) $(SESS_TARGET) $(UM_TARGET) $(PM_TARGET) $(CS_TARGET) $(LS_TARGET) $(STM_TARGET) $(NM_TARGET) $(UPM_TARGET) $(NS_TARGET) $(AM_TARGET) $(PRM_TARGET) $(SES_TARGET) $(COMP_TARGET) $(DS_TARGET)

# Windows Compatibility Subsystem Target
COMPAT_DIR := compat
WIN_RUNNER_TARGET := $(BUILD_DIR)/win_runner
COMPAT_SRCS := $(COMPAT_DIR)/win_runner.cpp $(COMPAT_DIR)/pe_loader.cpp $(COMPAT_DIR)/win32_translator.cpp $(COMPAT_DIR)/dx_translator.cpp $(COMPAT_DIR)/registry_virtualizer.cpp $(COMPAT_DIR)/path_redirector.cpp

$(WIN_RUNNER_TARGET):$(COMPAT_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Windows Compatibility Subsystem Runtime (win_runner)..."
	$(CXX)$(CXXFLAGS) -o $(WIN_RUNNER_TARGET)$(COMPAT_SRCS)

# Android Compatibility Subsystem Target
ANDROID_RUNNER_TARGET := $(BUILD_DIR)/android_runner
ANDR_SRCS := $(COMPAT_DIR)/android_runner.cpp $(COMPAT_DIR)/apk_parser.cpp $(COMPAT_DIR)/android_container.cpp $(COMPAT_DIR)/android_graphics_bridge.cpp $(COMPAT_DIR)/android_input_bridge.cpp $(COMPAT_DIR)/android_binder_bridge.cpp

$(ANDROID_RUNNER_TARGET):$(ANDR_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Android Compatibility Subsystem Runtime (android_runner)..."
	$(CXX)$(CXXFLAGS) -o $(ANDROID_RUNNER_TARGET)$(ANDR_SRCS)

# Terminal App Target
TERM_TARGET := $(BUILD_DIR)/terminal

$(TERM_TARGET): ../apps/terminal.cpp ../apps/terminal.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Terminal Emulator..."
	$(CXX) $(CXXFLAGS) -o $(TERM_TARGET) ../apps/terminal.cpp

# Append to all target (Hack to extend the current Makefile without rewriting everything)
all: $(TERM_TARGET)

# Task Manager App Target
TM_TARGET := $(BUILD_DIR)/task_manager

$(TM_TARGET): ../apps/task_manager.cpp ../apps/task_manager.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Task Manager..."
	$(CXX) $(CXXFLAGS) -o $(TM_TARGET) ../apps/task_manager.cpp

# Append to all target
all: $(TM_TARGET)

# Settings App Target
SET_TARGET := $(BUILD_DIR)/settings

$(SET_TARGET): ../apps/settings.cpp ../apps/settings.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ System Settings UI..."
	$(CXX) $(CXXFLAGS) -o $(SET_TARGET) ../apps/settings.cpp

# Append to all target
all: $(SET_TARGET)

# Calculator App Target
CALC_TARGET := $(BUILD_DIR)/calculator

$(CALC_TARGET): ../apps/calculator.cpp ../apps/calculator.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Calculator App..."
	$(CXX) $(CXXFLAGS) -o $(CALC_TARGET) ../apps/calculator.cpp

# Append to all target
all: $(CALC_TARGET)

# Text Editor App Target
TE_TARGET := $(BUILD_DIR)/text_editor

$(TE_TARGET): ../apps/text_editor.cpp ../apps/text_editor.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Text Editor App..."
	$(CXX) $(CXXFLAGS) -o $(TE_TARGET) ../apps/text_editor.cpp

# Append to all target
all: $(TE_TARGET)

# Media Player App Target
MP_TARGET := $(BUILD_DIR)/media_player

$(MP_TARGET): ../apps/media_player.cpp ../apps/media_player.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Media Player App..."
	$(CXX) $(CXXFLAGS) -o $(MP_TARGET) ../apps/media_player.cpp

# Append to all target
all: $(MP_TARGET)

# Image Viewer App Target
IV_TARGET := $(BUILD_DIR)/image_viewer

$(IV_TARGET): ../apps/image_viewer.cpp ../apps/image_viewer.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Image Viewer App..."
	$(CXX) $(CXXFLAGS) -o $(IV_TARGET) ../apps/image_viewer.cpp

# Append to all target
all: $(IV_TARGET)

# System Task Manager Utility Target
SYS_TM_TARGET := $(BUILD_DIR)/sys_task_manager

$(SYS_TM_TARGET): ../utilities/task_manager.cpp ../utilities/task_manager.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling Advanced System Task Manager Utility..."
	$(CXX) $(CXXFLAGS) -o $(SYS_TM_TARGET) ../utilities/task_manager.cpp

# Append to all target
all: $(SYS_TM_TARGET)

# System Device Manager Utility Target
SYS_DM_TARGET := $(BUILD_DIR)/sys_device_manager

$(SYS_DM_TARGET): ../utilities/device_manager.cpp ../utilities/device_manager.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling Advanced System Device Manager Utility..."
	$(CXX) $(CXXFLAGS) -o $(SYS_DM_TARGET) ../utilities/device_manager.cpp

# Append to all target
all: $(SYS_DM_TARGET)

# System Disk Manager Utility Target
SYS_DMGMT_TARGET := $(BUILD_DIR)/sys_disk_manager

$(SYS_DMGMT_TARGET): ../utilities/disk_manager.cpp ../utilities/disk_manager.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling Advanced System Disk Management Utility..."
	$(CXX) $(CXXFLAGS) -o $(SYS_DMGMT_TARGET) ../utilities/disk_manager.cpp

# Append to all target
all: $(SYS_DMGMT_TARGET)

# System Terminal Utility Target
SYS_TERM_TARGET := $(BUILD_DIR)/sys_terminal

$(SYS_TERM_TARGET): ../utilities/system_terminal.cpp ../utilities/system_terminal.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling System Administration Terminal Utility..."
	$(CXX) $(CXXFLAGS) -o $(SYS_TERM_TARGET) ../utilities/system_terminal.cpp

# Append to all target
all: $(SYS_TERM_TARGET)

# System Settings Utility Target
SYS_SETTINGS_TARGET := $(BUILD_DIR)/sys_settings_app

$(SYS_SETTINGS_TARGET): ../utilities/system_settings.cpp ../utilities/system_settings.h
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling Comprehensive System Settings Utility..."
	$(CXX) $(CXXFLAGS) -o $(SYS_SETTINGS_TARGET) ../utilities/system_settings.cpp

# Append to all target
all: $(SYS_SETTINGS_TARGET)
