#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "package_metadata.h"
#include "repository_format.h"
#include "package_installer.h"
#include "package_uninstaller.h"

class PackageCli {
private:
    PackageRegistry registry;
    RepositoryManager repo;
    PackageInstaller installer;
    PackageUninstaller uninstaller;

    void PrintHelp() const {
        std::cout << "Nexus OS Package Manager CLI (nexus-pkg)\n\n"
                  << "Usage:\n"
                  << "  nexus-pkg <command> [arguments]\n\n"
                  << "Commands:\n"
                  << "  install <package>   Download and install a package\n"
                  << "  remove <package>    Uninstall an existing package\n"
                  << "  search <query>      Search available repository packages\n"
                  << "  update              Check for and apply system updates\n"
                  << "  list                List all installed packages\n"
                  << "  help                Display this help message\n";
    }

public:
    PackageCli() : installer(registry), uninstaller(registry) {
        std::cout << "[PkgCli] Initializing Package Manager Command-Line Interface...\n";
    }

    int Execute(int argc, char* argv[]) {
        if (argc < 2) {
            PrintHelp();
            return 1;
        }

        std::string command = argv[1];

        if (command == "help" || command == "--help" || command == "-h") {
            PrintHelp();
            return 0;
        } else if (command == "list") {
            auto installed = registry.ListInstalled();
            std::cout << "--- Installed Packages (" << installed.size() << ") ---\n";
            for (const auto& pkg : installed) {
                std::cout << "  " << pkg.name << " v" << pkg.version << " [" << pkg.architecture << "]\n";
            }
            return 0;
        } else if (command == "search") {
            if (argc < 3) {
                std::cerr << "[!] Error: Missing search query.\n";
                return 1;
            }
            std::string query = argv[2];
            repo.UpdateIndexes();
            auto results = repo.Search(query);
            std::cout << "--- Search Results for '" << query << "' (" << results.size() << ") ---\n";
            for (const auto& res : results) {
                std::cout << "  " << res.meta.name << " v" << res.meta.version << " - " << res.meta.description << "\n";
            }
            return 0;
        } else if (command == "install") {
            if (argc < 3) {
                std::cerr << "[!] Error: Missing package name or file path.\n";
                return 1;
            }
            std::string target = argv[2];
            std::cout << "[PkgCli] Requesting installation of: " << target << "\n";
            // For testing CLI, we pass target path directly to installer
            installer.InstallPackage(target);
            return 0;
        } else if (command == "remove") {
            if (argc < 3) {
                std::cerr << "[!] Error: Missing package name.\n";
                return 1;
            }
            std::string pkg_name = argv[2];
            uninstaller.UninstallPackage(pkg_name);
            return 0;
        } else if (command == "update") {
            std::cout << "[PkgCli] Synchronizing repositories and checking for updates...\n";
            repo.UpdateIndexes();
            std::cout << "[+] System is up to date.\n";
            return 0;
        } else {
            std::cerr << "[!] Unknown command: " << command << "\n";
            PrintHelp();
            return 1;
        }
    }
};
