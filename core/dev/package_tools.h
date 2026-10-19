#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct PackageManifest {
    std::string name;
    std::string version;
    std::string architecture;
    std::string maintainer;
    std::string description;
    std::vector<std::string> dependencies;
};

class PackageBuilder {
private:
    std::string project_dir;

public:
    PackageBuilder(const std::string& directory = ".");

    // Scaffolds a new package directory structure and default manifest
    bool ScaffoldProject(const std::string& package_name);

    // Validates the manifest and directory structure before building
    bool LintPackage();

    // Compresses the package directory into a .npk (Nexus Package) distribution file
    bool BuildPackage(const std::string& output_dir);
};
