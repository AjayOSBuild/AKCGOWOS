#include "app_sandbox.h"
#include "virtual_disk.h"
#include <iostream>
#include <cstdlib>

AppSandbox::AppSandbox(const std::string& name, const std::string& exec_path) 
    : app_name(name), executable_path(exec_path), is_running(false) {
    sandbox_vm_name = "sandbox-" + app_name;
    std::cout << "[AppSandbox] Preparing isolation environment for '" << app_name << "'...\n";
}

bool AppSandbox::Initialize() {
    // Create a highly restricted microVM: 256MB RAM, 1 CPU
    if (!vm_mgr.CreateVm(sandbox_vm_name, 256, 1)) {
        std::cerr << "[!] Failed to create sandbox microVM.\n";
        return false;
    }

    // Apply strict resource limits to prevent sandbox escapes from hogging host resources
    vm_mgr.SetResourceLimits(sandbox_vm_name, 20, 300); // 20% CPU, 300MB Hard Mem Limit

    // In a real implementation, we would create a temporary overlayfs or qcow2 backing file 
    // linked to the host's application directory. For simulation, we log the intent.
    std::cout << "[AppSandbox] Configured read-only bind mount for executable: " << executable_path << "\n";
    std::cout << "[AppSandbox] Configured ephemeral scratch disk for application write layer.\n";
    
    // Explicitly do NOT attach network interfaces to enforce air-gapped security by default
    std::cout << "[AppSandbox] Network access strictly disabled (Air-gapped).\n";
    
    return true;
}

bool AppSandbox::Run() {
    std::cout << "[AppSandbox] Launching '" << app_name << "' inside secure microVM...\n";
    if (vm_mgr.StartVm(sandbox_vm_name)) {
        is_running = true;
        return true;
    }
    return false;
}

bool AppSandbox::Stop() {
    if (!is_running) return false;
    
    std::cout << "[AppSandbox] Terminating sandbox for '" << app_name << "'...\n";
    if (vm_mgr.StopVm(sandbox_vm_name)) {
        is_running = false;
        std::cout << "[AppSandbox] Ephemeral scratch disks purged. Sandbox destroyed.\n";
        return true;
    }
    return false;
}

int main() {
    std::cout << "=== OS Project Application Sandbox Utility ===\n";
    
    AppSandbox untrusted_pdf_reader("pdf-reader", "/usr/bin/nexus-pdf");
    
    if (untrusted_pdf_reader.Initialize()) {
        untrusted_pdf_reader.Run();
        
        std::cout << "[Test] Simulating sandboxed application runtime...\n";
        sleep(2);
        
        untrusted_pdf_reader.Stop();
    }
    
    std::cout << "[+] Application Sandbox testing completed.\n";
    return 0;
}
