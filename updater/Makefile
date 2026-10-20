CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

UP_CLIENT_TARGET = $(BUILD_DIR)/update_client
UP_CLIENT_SRCS = update_client.cpp

UP_SIG_TARGET = $(BUILD_DIR)/signature_verifier
UP_SIG_SRCS = signature_verifier.cpp

UP_SLOT_TARGET = $(BUILD_DIR)/slot_manager
UP_SLOT_SRCS = slot_manager.cpp

UP_DELTA_TARGET = $(BUILD_DIR)/delta_patcher
UP_DELTA_SRCS = delta_patcher.cpp

all: $(UP_CLIENT_TARGET) $(UP_SIG_TARGET) $(UP_SLOT_TARGET) $(UP_DELTA_TARGET)

$(UP_CLIENT_TARGET): $(UP_CLIENT_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Update Client..."
	$(CXX) $(CXXFLAGS) -o $(UP_CLIENT_TARGET) $(UP_CLIENT_SRCS)

$(UP_SIG_TARGET): $(UP_SIG_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Signature Verifier..."
	$(CXX) $(CXXFLAGS) -o $(UP_SIG_TARGET) $(UP_SIG_SRCS)

$(UP_SLOT_TARGET): $(UP_SLOT_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ A/B Slot Manager..."
	$(CXX) $(CXXFLAGS) -o $(UP_SLOT_TARGET) $(UP_SLOT_SRCS)

$(UP_DELTA_TARGET): $(UP_DELTA_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Delta Patcher..."
	$(CXX) $(CXXFLAGS) -o $(UP_DELTA_TARGET) $(UP_DELTA_SRCS)

clean:
	rm -f $(UP_CLIENT_TARGET) $(UP_SIG_TARGET) $(UP_SLOT_TARGET) $(UP_DELTA_TARGET)
