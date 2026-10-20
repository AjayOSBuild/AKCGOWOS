#include "user_creator.h"
#include <iostream>
#include <regex>
#include <cstdlib>

UserCreator::UserCreator(const std::string& root_path) : target_root_path(root_path) {
    std::cout << "[UserCreator] Initializing installer user configuration module...\n";
}

bool UserCreator::ValidateUser(const UserAccount& user, std::string& out_reason) const {
    if (user.username.empty()) {
        out_reason = "Username cannot be empty.";
        return false;
    }

    // POSIX username convention: lowercase letter at start, followed by lowercase letters, digits, underscores, or dashes
    std::regex username_pattern("^[a-z_][a-z0-9_-]*$");
    if (!std::regex_match(user.username, username_pattern)) {
        out_reason = "Invalid username format. Must start with a lowercase letter and contain only lowercase alphanumeric characters, dashes, or underscores.";
        return false;
    }

    if (user.password.length() < 6) {
        out_reason = "Password is too weak. Minimum length is 6 characters.";
        return false;
    }

    out_reason = "User account parameters are valid.";
    return true;
}

bool UserCreator::CreateUser(const UserAccount& user) const {
    std::string reason;
    if (!ValidateUser(user, reason)) {
        std::cerr << "[!] User Validation Failed: " << reason << "\n";
        return false;
    }

    std::cout << "[UserCreator] Configuring user '" << user.username << "' on target root: " << target_root_path << "\n";

    // In a live deployment, this runs inside a chroot environment against the target disk
    std::string cmd = "chroot " + target_root_path + " useradd -m -s /bin/bash -c \"" + user.fullname + "\" " + user.username;
    
    std::cout << "  -> [Safe Simulation] Would execute: " << cmd << "\n";
    if (user.is_admin) {
        std::cout << "  -> [Safe Simulation] Would assign administrator/sudo privileges to " << user.username << "\n";
    }

    std::cout << "[+] User account successfully configured.\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Installer: User Creation Utility ===\n";
    
    UserCreator creator("/mnt/nexus");
    UserAccount test_user = {"ajay", "Ajay Chandora", "securepassword123", true};
    
    creator.CreateUser(test_user);

    std::cout << "[+] User creation testing completed safely.\n";
    return 0;
}
