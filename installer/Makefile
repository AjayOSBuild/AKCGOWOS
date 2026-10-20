CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

INST_DISK_TARGET = $(BUILD_DIR)/disk_discovery
INST_DISK_SRCS = src/disk_discovery.cpp

INST_DISPLAY_TARGET = $(BUILD_DIR)/partition_display
INST_DISPLAY_SRCS = src/partition_display.cpp src/disk_discovery.cpp

all: $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET)

$(INST_DISK_TARGET): $(INST_DISK_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(INST_DISK_TARGET) $(INST_DISK_SRCS)

$(INST_DISPLAY_TARGET): $(INST_DISPLAY_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Partition Display Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_DISPLAY_TARGET) $(INST_DISPLAY_SRCS)

clean:
	rm -f $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET)

INST_TARGET_TARGET = $(BUILD_DIR)/target_selector
INST_TARGET_SRCS = src/target_selector.cpp src/disk_discovery.cpp

# Append to all target
all: $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET)

$(INST_TARGET_TARGET): $(INST_TARGET_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Target Selector Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_TARGET_TARGET) $(INST_TARGET_SRCS)

clean:
	rm -f $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET)

INST_FS_TARGET = $(BUILD_DIR)/filesystem_creator
INST_FS_SRCS = src/filesystem_creator.cpp

# Append to all target
all: $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET)

$(INST_FS_TARGET): $(INST_FS_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Filesystem Creator Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_FS_TARGET) $(INST_FS_SRCS)

clean:
	rm -f $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET)

INST_USER_TARGET = $(BUILD_DIR)/user_creator
INST_USER_SRCS = src/user_creator.cpp

# Append to all target
all: $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET) $(INST_USER_TARGET)

$(INST_USER_TARGET): $(INST_USER_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ User Creator Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_USER_TARGET) $(INST_USER_SRCS)

clean:
	rm -f $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET) $(INST_USER_TARGET)

INST_NET_TARGET = $(BUILD_DIR)/network_config
INST_NET_SRCS = src/network_config.cpp

# Append to all target
all: $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET) $(INST_USER_TARGET) $(INST_NET_TARGET)

$(INST_NET_TARGET): $(INST_NET_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Network Configurer Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_NET_TARGET) $(INST_NET_SRCS)

clean:
	rm -f $(INST_DISK_TARGET) $(INST_DISPLAY_TARGET) $(INST_TARGET_TARGET) $(INST_FS_TARGET) $(INST_USER_TARGET) $(INST_NET_TARGET)
