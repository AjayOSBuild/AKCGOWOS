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
	$(MAKE) -C core

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

UP_SIG_TARGET = $(BUILD_DIR)/signature_verifier
UP_SIG_SRCS = signature_verifier.cpp

# Append to all target
all: $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

$(UP_SIG_TARGET): $(UP_SIG_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Signature Verifier..."
	$(CXX) $(CXXFLAGS) -o $(UP_SIG_TARGET) $(UP_SIG_SRCS)

clean:
	rm -f $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

UP_SIG_TARGET = $(BUILD_DIR)/signature_verifier
UP_SIG_SRCS = signature_verifier.cpp

# Append to all target
all: $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

$(UP_SIG_TARGET): $(UP_SIG_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Signature Verifier..."
	$(CXX) $(CXXFLAGS) -o $(UP_SIG_TARGET) $(UP_SIG_SRCS)

clean:
	rm -f $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

UP_SIG_TARGET = $(BUILD_DIR)/signature_verifier
UP_SIG_SRCS = signature_verifier.cpp

# Append to all target
all: $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

$(UP_SIG_TARGET): $(UP_SIG_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Signature Verifier..."
	$(CXX) $(CXXFLAGS) -o $(UP_SIG_TARGET) $(UP_SIG_SRCS)

clean:
	rm -f $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)
