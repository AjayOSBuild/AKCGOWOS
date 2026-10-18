#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;

class FileTrashManager {
private:
    std::string trash_dir;
    std::string info_dir;

public:
    FileTrashManager(const std::string& baseDir = "/root/.local/share/os/trash") 
        : trash_dir(baseDir + "/files"), info_dir(baseDir + "/info") {
        std::error_code ec;
        fs::create_directories(trash_dir, ec);
        fs::create_directories(info_dir, ec);
        std::cout << "[FileTrashManager] Subsystem initialized. Trash path: " << trash_dir << "\n";
    }

    bool MoveToTrash(const std::string& filePath) {
        if (!fs::exists(filePath)) {
            std::cerr << "[!] Cannot trash file. Path not found: " << filePath << "\n";
            return false;
        }

        std::string filename = fs::path(filePath).filename().string();
        std::string timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
        std::string uniqueName = timestamp + "_" + filename;

        std::string destFilePath = (fs::path(trash_dir) / uniqueName).string();
        std::string infoFilePath = (fs::path(info_dir) / (uniqueName + ".trashinfo")).string();

        std::error_code ec;
        fs::rename(filePath, destFilePath, ec);
        if (ec) {
            std::cerr << "[!] Failed to move file to trash: " << ec.message() << "\n";
            return false;
        }

        // Write metadata info file (.trashinfo format)
        std::ofstream infoFile(infoFilePath);
        if (infoFile.is_open()) {
            infoFile << "[Trash Info]\n";
            infoFile << "Path=" << filePath << "\n";
            infoFile << "DeletionDate=" << timestamp << "\n";
            infoFile.close();
        }

        std::cout << "[FileTrashManager] Safely moved to trash: " << filePath << " -> " << destFilePath << "\n";
        return true;
    }

    bool RestoreFromTrash(const std::string& uniqueName) {
        std::string targetFile = (fs::path(trash_dir) / uniqueName).string();
        std::string infoFile = (fs::path(info_dir) / (uniqueName + ".trashinfo")).string();

        if (!fs::exists(targetFile) || !fs::exists(infoFile)) {
            std::cerr << "[!] Trash item or metadata not found for: " << uniqueName << "\n";
            return false;
        }

        // Parse original path from .trashinfo
        std::ifstream ifs(infoFile);
        std::string originalPath = "";
        std::string line;
        while (std::getline(ifs, line)) {
            if (line.rfind("Path=", 0) == 0) {
                originalPath = line.substr(5);
                break;
            }
        }
        ifs.close();

        if (originalPath.empty()) {
            std::cerr << "[!] Could not resolve original path from trash info.\n";
            return false;
        }

        std::error_code ec;
        fs::create_directories(fs::path(originalPath).parent_path(), ec);
        fs::rename(targetFile, originalPath, ec);
        if (ec) {
            std::cerr << "[!] Failed to restore file: " << ec.message() << "\n";
            return false;
        }

        fs::remove(infoFile, ec);
        std::cout << "[FileTrashManager] Successfully restored item to: " << originalPath << "\n";
        return true;
    }

    bool EmptyTrash() {
        std::error_code ec;
        fs::remove_all(trash_dir, ec);
        fs::remove_all(info_dir, ec);
        fs::create_directories(trash_dir, ec);
        fs::create_directories(info_dir, ec);
        std::cout << "[FileTrashManager] Trash emptied successfully.\n";
        return true;
    }
};
