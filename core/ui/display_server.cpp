#include "display_server.h"

DisplayServer::DisplayServer(const std::string& ipc_socket_path) : socket_path(ipc_socket_path) {
    std::cout << "[DisplayServer] Initializing Display Server IPC Subsystem...\n";
}

bool DisplayServer::StartServer() {
    std::cout << "[DisplayServer] Binding Unix Domain Socket at " << socket_path << "...\n";
    // In production, this binds socket() and enters an epoll()/poll() event loop.
    std::cout << "  -> Listening for incoming graphical application connections.\n";
    return true;
}

uint32_t DisplayServer::ConnectClient(const std::string& process_name) {
    uint32_t cid = next_client_id++;
    connected_clients[cid] = {cid, process_name, {}};
    std::cout << "[DisplayServer] Client Connected: " << process_name << " (ID: " << cid << ")\n";
    return cid;
}

uint32_t DisplayServer::AllocateSurface(uint32_t client_id, uint32_t width, uint32_t height) {
    if (connected_clients.find(client_id) == connected_clients.end()) {
        std::cerr << "  -> [!] Allocation failed: Unknown client ID " << client_id << "\n";
        return 0;
    }

    uint32_t sid = next_surface_id++;
    
    // Simulate mapping a memfd or POSIX shm descriptor
    int mock_shm_fd = 100 + sid; 
    
    surface_pool[sid] = {sid, mock_shm_fd, width, height, 0x34325241}; // Mock ARGB format
    connected_clients[client_id].active_surfaces.push_back(sid);
    
    std::cout << "  -> Allocated " << width << "x" << height << " surface (ID: " << sid 
              << ", shm_fd: " << mock_shm_fd << ") for Client " << client_id << "\n";
    return sid;
}

void DisplayServer::DisconnectClient(uint32_t client_id) {
    auto it = connected_clients.find(client_id);
    if (it != connected_clients.end()) {
        std::cout << "[DisplayServer] Disconnecting Client " << client_id << "...\n";
        
        // Cleanup associated surfaces
        for (uint32_t sid : it->second.active_surfaces) {
            surface_pool.erase(sid);
            std::cout << "  -> Destroyed Surface ID: " << sid << "\n";
        }
        
        connected_clients.erase(it);
        std::cout << "  -> Client resources freed.\n";
    }
}

void DisplayServer::DumpServerState() const {
    std::cout << "\n--- Display Server State ---\n";
    std::cout << "Connected Clients: " << connected_clients.size() << "\n";
    for (const auto& pair : connected_clients) {
        const auto& client = pair.second;
        std::cout << "  - ID: " << client.client_id << " | Process: " << client.process_name 
                  << " | Active Surfaces: " << client.active_surfaces.size() << "\n";
    }
    std::cout << "Total Active Surfaces (Shared Memory): " << surface_pool.size() << "\n";
    std::cout << "----------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project Display Server Utility ===\n";
    DisplayServer display;
    display.StartServer();

    // Simulate the Package Manager GUI and a Terminal app connecting
    uint32_t pkg_gui_id = display.ConnectClient("pkg_gui");
    display.AllocateSurface(pkg_gui_id, 900, 700);

    uint32_t term_id = display.ConnectClient("nexus_term");
    display.AllocateSurface(term_id, 800, 600);
    display.AllocateSurface(term_id, 640, 480); // Terminal opened a second window

    display.DumpServerState();

    // Simulate Terminal closing
    display.DisconnectClient(term_id);
    display.DumpServerState();

    std::cout << "[+] Display Server testing completed successfully.\n";
    return 0;
}
