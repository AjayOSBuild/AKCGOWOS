#pragma once
#include "../core/ui_toolkit.h"
#include "../core/file_indexer.h"
#include "../core/file_metadata.h"
#include "../core/file_trash.h"
#include <memory>
#include <vector>
#include <fstream>
#include <filesystem>

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
            std::cout << "[FileExplorer] Switched view to Recycle Bin.\n";
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
        std::cout << "[FileExplorer] Refreshed view. Loaded " << current_contents.size() << " entries.\n";
    }

    // EXP-011: Load and parse Recycle Bin items and .trashinfo metadata
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

                // Parse corresponding .trashinfo file
                std::string infoPath = (fs::path(trash_info_dir) / (item.unique_name + ".trashinfo")).string();
                if (fs::exists(infoPath)) {
                    std::ifstream ifs(infoPath);
                    std::string line;
                    while (std::getline(ifs, line)) {
                        if (line.rfind("Path=", 0) == 0) {
                            item.original_path = line.substr(5);
                        } else if (line.rfind("DeletionDate=", 0) == 0) {
                            item.deletion_date = line.substr(13);
                        }
                    }
                    ifs.close();
                } else {
                    item.original_path = "Unknown";
                    item.deletion_date = "Unknown";
                }

                trash_contents.push_back(item);
            }
        }
        std::cout << "[FileExplorer] Recycle bin loaded. Found " << trash_contents.size() << " deleted items.\n";
    }

    bool RestoreItemFromTrash(const std::string& unique_name) {
        bool success = trash_manager.RestoreFromTrash(unique_name);
        if (success && is_trash_view) {
            RefreshTrashContents();
        }
        return success;
    }

    bool ClearRecycleBin() {
        bool success = trash_manager.EmptyTrash();
        if (success && is_trash_view) {
            RefreshTrashContents();
        }
        return success;
    }

    void HandleAddressBarInput(const std::string& typed_path) {
        std::cout << "[FileExplorer] Address bar submission: " << typed_path << "\n";
        NavigateTo(typed_path);
    }

    bool RenameItem(const std::string& old_path, const std::string& new_name) {
        fs::path src(old_path);
        fs::path dest = src.parent_path() / new_name;
        std::error_code ec;
        
        fs::rename(src, dest, ec);
        if (ec) {
            std::cerr << "[!] Rename operation failed: " << ec.message() << "\n";
            return false;
        }
        
        std::cout << "[FileExplorer] Successfully renamed " << src.string() << " -> " << dest.string() << "\n";
        RefreshContents();
        return true;
    }

    bool CopyItem(const std::string& source, const std::string& destination) {
        std::error_code ec;
        fs::copy(source, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
        if (ec) {
            std::cerr << "[!] Copy operation failed: " << ec.message() << "\n";
            return false;
        }
        std::cout << "[FileExplorer] Successfully copied " << source << " -> " << destination << "\n";
        RefreshContents();
        return true;
    }

    bool MoveItem(const std::string& source, const std::string& destination) {
        std::error_code ec;
        fs::rename(source, destination, ec);
        if (ec) {
            std::cerr << "[!] Move operation failed: " << ec.message() << "\n";
            return false;
        }
        std::cout << "[FileExplorer] Successfully moved " << source << " -> " << destination << "\n";
        RefreshContents();
        return true;
    }

    bool DeleteToTrash(const std::string& path) {
        bool success = trash_manager.MoveToTrash(path);
        if (success && !is_trash_view) {
            RefreshContents();
        }
        return success;
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 30, 200, 738, 0xFFE0E0E0);
        gfx.DrawRect(0, 30, 200, 738, 0xFF888888, 1);
        
        gfx.FillRect(200, 30, 824, 738, 0xFFFFFFFF);
        
        font_engine.DrawText("Quick Access", 10, 50, 12, 0xFF000000);
        font_engine.DrawText("Volumes (/mnt)", 10, 80, 12, 0xFF000000);
        font_engine.DrawText("Recycle Bin (trash://)", 10, 110, 12, 0xFF000000);

        font_engine.DrawText("Path: " + current_path, 210, 50, 12, 0xFF000000);

        int start_y = 80;
        if (is_trash_view) {
            for (const auto& item : trash_contents) {
                std::string display_str = "[TRASH] " + item.unique_name + " (Original: " + item.original_path + ")";
                font_engine.DrawText(display_str, 210, start_y, 12, 0xFF000000);
                start_y += 20;
                if (start_y > 720) break;
            }
        } else {
            for (const auto& item : current_contents) {
                std::string prefix = item.is_directory ? "[DIR]  " : "[FILE] ";
                font_engine.DrawText(prefix + item.name, 210, start_y, 12, 0xFF000000);
                start_y += 20;
                if (start_y > 720) break;
            }
        }
    }
    
    void HandleSearch(const std::string& query) {
        auto results = indexer.Search(query);
        std::cout << "[FileExplorer] Search query '" << query << "' returned " << results.size() << " results.\n";
    }
};
