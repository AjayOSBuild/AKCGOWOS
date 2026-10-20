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
