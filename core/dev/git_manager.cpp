#include "git_manager.h"
#include <cstdlib>
#include <sstream>
#include <array>
#include <memory>

GitManager::GitManager(const std::string& path) : repo_path(path) {
    std::cout << "[GitManager] Initialized for repository path: " << repo_path << "\n";
}

bool GitManager::Clone(const std::string& repo_url, const std::string& destination) {
    std::string cmd = "git clone " + repo_url + " " + destination;
    std::cout << "[GitManager] Executing: " << cmd << "\n";
    return system(cmd.c_str()) == 0;
}

std::string GitManager::Status() {
    std::string cmd = "git -C " + repo_path + " status --porcelain";
    std::array<char, 128> buffer;
    std::string result = "";
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR: failed to run git status";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

bool GitManager::Commit(const std::string& message) {
    std::string add_cmd = "git -C " + repo_path + " add .";
    std::string commit_cmd = "git -C " + repo_path + " commit -m \"" + message + "\"";
    std::cout << "[GitManager] Executing: " << add_cmd << " && " << commit_cmd << "\n";
    
    int ret1 = system(add_cmd.c_str());
    if (ret1 != 0) {
        std::cerr << "[!] git add failed.\n";
        return false;
    }
    return system(commit_cmd.c_str()) == 0;
}

bool GitManager::Push(const std::string& remote, const std::string& branch) {
    std::string cmd = "git -C " + repo_path + " push " + remote + " " + branch;
    std::cout << "[GitManager] Executing: " << cmd << "\n";
    return system(cmd.c_str()) == 0;
}

bool GitManager::CheckoutBranch(const std::string& branch_name, bool create) {
    std::string cmd = "git -C " + repo_path + " checkout " + (create ? "-b " : "") + branch_name;
    std::cout << "[GitManager] Executing: " << cmd << "\n";
    return system(cmd.c_str()) == 0;
}
