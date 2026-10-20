#pragma once
#include <string>

struct UserAccount {
    std::string username;
    std::string fullname;
    std::string password;
    bool is_admin;
};

class UserCreator {
private:
    std::string target_root_path;

public:
    explicit UserCreator(const std::string& root_path = "/mnt/nexus");

    // Validates username convention and password complexity rules
    bool ValidateUser(const UserAccount& user, std::string& out_reason) const;

    // Creates the user account within the target mounted root filesystem safely
    bool CreateUser(const UserAccount& user) const;
};
