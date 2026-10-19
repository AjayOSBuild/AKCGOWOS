#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdio>

class GitManager {
private:
    std::string repo_path;

public:
    GitManager(const std::string& path = ".");
    bool Clone(const std::string& repo_url, const std::string& destination);
    std::string Status();
    bool Commit(const std::string& message);
    bool Push(const std::string& remote = "origin", const std::string& branch = "main");
    bool CheckoutBranch(const std::string& branch_name, bool create = false);
};
