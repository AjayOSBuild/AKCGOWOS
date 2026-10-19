#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

template <typename RegistryType, typename RepoType>
class DependencyResolver {
private:
    RegistryType& registry;
    RepoType& repo;

    bool ResolveRecursive(const std::string& pkg_name, std::vector<std::string>& plan, 
                          std::unordered_set<std::string>& visited, 
                          std::unordered_set<std::string>& visiting) {
        // Cycle detection
        if (visiting.find(pkg_name) != visiting.end()) {
            std::cerr << "[!] Circular dependency detected involving: " << pkg_name << "\n";
            return false;
        }
        
        // Skip already processed nodes in the dependency graph
        if (visited.find(pkg_name) != visited.end()) {
            return true;
        }

        // Check local registry state to skip redundant installations
        if (registry.IsInstalled(pkg_name)) {
            std::cout << "[Resolver] Package '" << pkg_name << "' is already installed. Skipping.\n";
            visited.insert(pkg_name);
            return true;
        }

        visiting.insert(pkg_name);

        // Fetch remote metadata for dependencies
        RemotePackageInfo info;
        if (!repo.GetPackageInfo(pkg_name, info)) {
            std::cerr << "[!] Dependency resolution failed: Package '" << pkg_name << "' not found in repositories.\n";
            return false;
        }

        // Recursively resolve child dependencies first (Post-order traversal)
        for (const auto& dep : info.meta.dependencies) {
            std::cout << "[Resolver] '" << pkg_name << "' depends on '" << dep << "'...\n";
            if (!ResolveRecursive(dep, plan, visited, visiting)) {
                return false;
            }
        }

        visiting.erase(pkg_name);
        visited.insert(pkg_name);
        
        // Add to install plan after all dependencies are satisfied
        plan.push_back(pkg_name);
        return true;
    }

public:
    DependencyResolver(RegistryType& reg, RepoType& r) : registry(reg), repo(r) {
        std::cout << "[Resolver] Initialized Package Dependency Resolver Engine.\n";
    }

    bool Resolve(const std::string& target_pkg, std::vector<std::string>& install_plan) {
        std::cout << "[Resolver] Calculating installation plan for: " << target_pkg << "\n";
        std::unordered_set<std::string> visited;
        std::unordered_set<std::string> visiting;
        
        bool success = ResolveRecursive(target_pkg, install_plan, visited, visiting);
        if (success) {
            std::cout << "[+] Dependency resolution complete. " << install_plan.size() << " packages queued for installation.\n";
        } else {
            std::cerr << "[!] Dependency resolution aborted.\n";
        }
        return success;
    }
};
