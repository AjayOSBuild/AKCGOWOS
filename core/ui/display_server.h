#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct SurfaceBuffer {
    uint32_t surface_id;
    int shm_fd;           // Shared memory file descriptor
    uint32_t width;
    uint32_t height;
    uint32_t format;      // Pixel format (e.g., ARGB8888)
};

struct UiClient {
    uint32_t client_id;
    std::string process_name;
    std::vector<uint32_t> active_surfaces;
};

class DisplayServer {
private:
    std::string socket_path;
    uint32_t next_client_id = 1000;
    uint32_t next_surface_id = 1;
    
    std::unordered_map<uint32_t, UiClient> connected_clients;
    std::unordered_map<uint32_t, SurfaceBuffer> surface_pool;

public:
    DisplayServer(const std::string& ipc_socket_path = "/run/os/display.sock");

    // Starts the IPC listener for incoming graphical applications
    bool StartServer();

    // Registers a new client connection
    uint32_t ConnectClient(const std::string& process_name);

    // Allocates a shared memory framebuffer for a client
    uint32_t AllocateSurface(uint32_t client_id, uint32_t width, uint32_t height);

    // Cleans up resources when a client disconnects
    void DisconnectClient(uint32_t client_id);
    
    // Displays currently tracked clients and surfaces
    void DumpServerState() const;
};
