#include <iostream>
#include <unordered_map>
#include "package_format.h"
#include "repository_format.h"
#include "dependency_resolver.h"

// --- Mock Classes to isolate testing of the DAG logic ---

class MockRegistry {
public:
    bool IsInstalled(const std::string& name) {
        // Pretend coreutils is already installed on the system
        return name == "coreutils"; 
    }
};

class MockRepo {
public:
    std::unordered_map<std::string, RemotePackageInfo> db;

    bool GetPackageInfo(const std::string& name, RemotePackageInfo& out_info) const {
        auto it = db.find(name);
        if (it != db.end()) {
            out_info = it->second;
            return true;
        }
        return false;
    }
};

int main() {
    std::cout << "=== OS Project Package Dependency Resolver Utility ===\n";
    
    MockRegistry registry;
    MockRepo repo;

    // Populate Mock Repo with a dependency tree
    RemotePackageInfo app, libssl, libc6, coreutils;
    app.meta.name = "nexus-browser";
    app.meta.dependencies = {"libssl", "libc6", "coreutils"}; // Depends on 3 packages
    
    libssl.meta.name = "libssl";
    libssl.meta.dependencies = {"libc6"}; // libssl itself depends on libc6
    
    libc6.meta.name = "libc6";
    libc6.meta.dependencies = {}; // Base library, no dependencies

    coreutils.meta.name = "coreutils";
    coreutils.meta.dependencies = {}; 
    
    repo.db["nexus-browser"] = app;
    repo.db["libssl"] = libssl;
    repo.db["libc6"] = libc6;
    repo.db["coreutils"] = coreutils;

    // Instantiate Resolver
    DependencyResolver<MockRegistry, MockRepo> resolver(registry, repo);
    std::vector<std::string> install_plan;
    
    std::cout << "\n--- Testing Dependency Resolution for 'nexus-browser' ---\n";
    if (resolver.Resolve("nexus-browser", install_plan)) {
        std::cout << "\n[+] Final Installation Plan (Execute Top to Bottom):\n";
        for (size_t i = 0; i < install_plan.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << install_plan[i] << "\n";
        }
    }

    std::cout << "\n[+] Dependency Resolver testing completed successfully.\n";
    return 0;
}
