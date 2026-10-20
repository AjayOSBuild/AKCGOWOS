CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

INST_DISK_TARGET = $(BUILD_DIR)/disk_discovery
INST_DISK_SRCS = src/disk_discovery.cpp

all: $(INST_DISK_TARGET)

$(INST_DISK_TARGET): $(INST_DISK_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Disk Discovery Utility..."
	$(CXX) $(CXXFLAGS) -o $(INST_DISK_TARGET) $(INST_DISK_SRCS)

clean:
	rm -f $(INST_DISK_TARGET)
