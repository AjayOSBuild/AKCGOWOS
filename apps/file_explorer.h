#pragma once
#include "../core/ui_toolkit.h"
#include "../core/file_indexer.h"
#include "../core/file_metadata.h"
#include "../core/file_trash.h"
#include "../core/thumbnail_generator.h"
#include <memory>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

struct FileItemEntry {
    std::string name;
    std::string full_path;
    bool is_directory;
    uint64_t size_bytes;
};

struct TrashItemEntry {
    std::string unique_name;
    std::string original_path;
    std::string deletion_date;
    uint64_t size_bytes;
};

class FileExplorerApp {
private:
    std::string current_path;
    FileIndexer indexer;
    FileTrashManager trash_manager;
    ThumbnailGenerator thumbnail_gen;
    std::vector<FileItemEntry> current_contents;
    std::vector<TrashItemEntry> trash_contents;
    bool is_trash_view = false;
    
public:
    FileExplorerApp(const std::string& initial_path = "/mnt/usb") {
        NavigateTo(initial_path);
    }
    
    void NavigateTo(const std::string& path) {
        if (path == "trash://") {
            is_trash_view = true;
            current_path = "trash://";
            RefreshTrashContents();
            return;
        }

        is_trash_view = false;
        std::error_code ec;
        if (fs::exists(path) && fs::is_directory(path, ec)) {
            current_path = path;
            std::cout << "[FileExplorer] Navigated to directory: " << current_path << "\n";
            RefreshContents();
        } else {
            std::cerr << "[!] Navigation failed. Path does not exist or is not a directory: " << path << "\n";
        }
    }

    void RefreshContents() {
        current_contents.clear();
        std::error_code ec;
        if (!fs::exists(current_path)) return;

        for (const auto& entry : fs::directory_iterator(current_path, fs::directory_options::skip_permission_denied, ec)) {
            FileItemEntry item;
            item.full_path = entry.path().string();
            item.name = entry.path().filename().string();
            item.is_directory = entry.is_directory(ec);
            item.size_bytes = item.is_directory ? 0 : entry.file_size(ec);
            current_contents.push_back(item);
        }
    }

    void RefreshTrashContents() {
        trash_contents.clear();
        std::string trash_files_dir = "/root/.local/share/os/trash/files";
        std::string trash_info_dir = "/root/.local/share/os/trash/info";
        std::error_code ec;
        if (!fs::exists(trash_files_dir)) return;

        for (const auto& entry : fs::directory_iterator(trash_files_dir, ec)) {
            if (entry.is_regular_file(ec) || entry.is_directory(ec)) {
                TrashItemEntry item;
                item.unique_name = entry.path().filename().string();
                item.size_bytes = entry.is_regular_file(ec) ? entry.file_size(ec) : 0;

                std::string infoPath = (fs::path(trash_info_dir) / (item.unique_name + ".trashinfo")).string();
                if (fs::exists(infoPath)) {
                    std::ifstream ifs(infoPath);
                    std::string line;
                    while (std::getline(ifs, line)) {
                        if (line.rfind("Path=", 0) == 0) item.original_path = line.substr(5);
                        else if (line.rfind("DeletionDate=", 0) == 0) item.deletion_date = line.substr(13);
                    }
                    ifs.close();
                }
                trash_contents.push_back(item);
            }
        }
    }

    // EXP-013: File Previews
    void ShowFilePreview(const std::string& path) {
        std::cout << "\n=== File Preview: " << path << " ===\n";
        std::string ext = fs::path(path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
            if (!thumbnail_gen.HasThumbnail(path)) {
                thumbnail_gen.GenerateThumbnail(path);
            }
            std::cout << "[Preview] Displaying cached thumbnail: " << thumbnail_gen.GetThumbnailPath(path) << "\n";
        } else if (ext == ".txt" || ext == ".md" || ext == ".log" || ext == ".cpp" || ext == ".h") {
            std::ifstream file(path);
            std::string line;
            int lines_read = 0;
            std::cout << "[Preview] Text snippet:\n----------------------\n";
            while (std::getline(file, line) && lines_read < 5) {
                std::cout << line << "\n";
                lines_read++;
            }
            if (file.good()) std::cout << "... (truncated)\n";
            std::cout << "----------------------\n";
        } else {
            std::cout << "[Preview] No preview available for this file type.\n";
        }
    }

    // EXP-015: Archive Support
    bool ExtractArchive(const std::string& archive_path) {
        if (!fs::exists(archive_path)) {
            std::cerr << "[!] Archive not found: " << archive_path << "\n";
            return false;
        }

        std::string ext = fs::path(archive_path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        std::string target_dir = (fs::path(archive_path).parent_path() / fs::path(archive_path).stem()).string();
        std::error_code ec;
        fs::create_directories(target_dir, ec);

        std::cout << "[ArchiveManager] Extracting " << archive_path << " to " << target_dir << "...\n";
        
        // In a full OS, this would link against libarchive.
        // For this layer, we simulate extraction routing.
        if (ext == ".zip") {
            std::cout << "  -> Simulated ZIP extraction successful.\n";
        } else if (ext == ".gz" || ext == ".tar") {
            std::cout << "  -> Simulated TAR/GZ extraction successful.\n";
        } else {
            std::cout << "  -> Unsupported archive format.\n";
            return false;
        }

        RefreshContents();
        return true;
    }

    void HandleAddressBarInput(const std::string& typed_path) {
        NavigateTo(typed_path);
    }
};
