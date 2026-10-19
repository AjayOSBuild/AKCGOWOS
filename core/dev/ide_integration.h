#pragma once
#include <iostream>
#include <string>
#include <vector>

// Defines the types of requests an IDE can send to the OS developer environment
enum class IdeRequestType {
    BUILD_PROJECT,
    START_DEBUGGER,
    LSP_DIAGNOSTICS,
    GIT_STATUS
};

struct IdeRequest {
    IdeRequestType type;
    std::string payload;
};

class IdeIntegrationBroker {
public:
    IdeIntegrationBroker();
    
    // Initializes IPC socket listener for IDE connections (e.g., /run/os/ide_broker.sock)
    bool InitializeIpcListener();

    // Routes parsed IDE requests to the CompilerManager, GdbManager, or GitManager
    std::string HandleRequest(const IdeRequest& request);

    // Simulated background Language Server Protocol (LSP) engine wrapper
    void StartLspBackend(const std::string& language);
};
