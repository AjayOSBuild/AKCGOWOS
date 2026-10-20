CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
BUILD_DIR = ../build

UP_CLIENT_TARGET = $(BUILD_DIR)/update_client
UP_CLIENT_SRCS = update_client.cpp

UP_SIG_TARGET = $(BUILD_DIR)/signature_verifier
UP_SIG_SRCS = signature_verifier.cpp

all: $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)

$(UP_CLIENT_TARGET): $(UP_CLIENT_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Update Client..."
	$(CXX) $(CXXFLAGS) -o $(UP_CLIENT_TARGET) $(UP_CLIENT_SRCS)

$(UP_SIG_TARGET): $(UP_SIG_SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "[*] Compiling C++ Signature Verifier..."
	$(CXX) $(CXXFLAGS) -o $(UP_SIG_TARGET) $(UP_SIG_SRCS)

clean:
	rm -f $(UP_CLIENT_TARGET) $(UP_SIG_TARGET)
