#include <iostream>
#include <string>
#include <map>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

enum class ServiceState { STOPPED, RUNNING, FAILED };

class Service {
public:
    std::string name;
    std::string command;
    pid_t pid = -1;
    ServiceState state = ServiceState::STOPPED;

    Service(std::string n, std::string cmd) : name(n), command(cmd) {}

    void start() {
        if (state == ServiceState::RUNNING) return;
        std::cout << "[ServiceManager] Starting service: " << name << "...\n";
        
        pid = fork();
        if (pid == 0) {
            // Child process execution
            execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
            // If execl fails:
            std::cerr << "[!] Failed to execute service: " << name << "\n";
            exit(1);
        } else if (pid > 0) {
            state = ServiceState::RUNNING;
        } else {
            std::cerr << "[!] Fork failed for service: " << name << "\n";
            state = ServiceState::FAILED;
        }
    }

    void stop() {
        if (state == ServiceState::RUNNING && pid > 0) {
            std::cout << "[ServiceManager] Stopping service: " << name << "...\n";
            kill(pid, SIGTERM);
            state = ServiceState::STOPPED;
            pid = -1;
        }
    }
};

int main() {
    std::cout << "\n=== OS Project Service Manager (Ring 3) ===\n";
    std::map<std::string, Service> services;

    // Define core service placeholders for Phase 4 validation
    services.emplace("DeviceManager", Service("DeviceManager", "/sbin/device_manager"));
    services.emplace("LoggingService", Service("LoggingService", "/sbin/logging_service"));
    services.emplace("StorageManager", Service("StorageManager", "/sbin/storage_manager"));
    services.emplace("NetworkManager", Service("NetworkManager", "/sbin/network_manager"));
    services.emplace("SessionManager", Service("SessionManager", "/sbin/session_manager"));
    services.emplace("UserManager", Service("UserManager", "/sbin/user_manager"));
    services.emplace("PermissionManager", Service("PermissionManager", "/sbin/permission_manager"));
    services.emplace("ConfigurationService", Service("ConfigurationService", "/sbin/configuration_service"));

    // Startup phase
    for (auto& pair : services) {
        pair.second.start();
    }

    std::cout << "[+] Service Manager initialization complete. Monitoring services...\n";
    
    // Monitoring and error handling loop
    int status;
    pid_t exited_pid;
    while ((exited_pid = wait(&status)) > 0) {
        for (auto& pair : services) {
            if (pair.second.pid == exited_pid) {
                std::cout << "[!] Service exited: " << pair.second.name << ". Restarting...\n";
                pair.second.state = ServiceState::FAILED;
                pair.second.start(); // Auto-restart handling
                break;
            }
        }
    }

    return 0;
}
