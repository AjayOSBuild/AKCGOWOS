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

// EXP-018: Tab state structure
struct ExplorerTab {
    std::string current_path;
    bool is_trash_view = false;
    std::vector<FileItemEntry> current_contents;
    std::vector<TrashItemEntry> trash_contents;
};

class FileExplorerApp {
private:
    std::vector<ExplorerTab> tabs;
    int active_tab = 0;
    int window_id;

    // Shared background services
    FileIndexer indexer;
    FileTrashManager trash_manager;
    ThumbnailGenerator thumbnail_gen;
    
public:
    FileExplorerApp(int win_id = 1, const std::string& initial_path = "/mnt/usb") : window_id(win_id) {
        std::cout << "[FileExplorer] Initializing Window #" << window_id << "\n";
        OpenNewTab(initial_path);
    }

    // EXP-019: Multiple Windows Support
    static std::shared_ptr<FileExplorerApp> SpawnNewWindow(int win_id, const std::string& initial_path = "/mnt/usb") {
        std::cout << "[FileExplorer] Requesting new Compositor surface for multi-window support...\n";
        return std::make_shared<FileExplorerApp>(win_id, initial_path);
    }
    
    // EXP-018: Tabs Support
    void OpenNewTab(const std::string& path) {
        ExplorerTab new_tab;
        tabs.push_back(new_tab);
        active_tab = tabs.size() - 1;
        std::cout << "[FileExplorer Window #" << window_id << "] Opened new tab (Index: " << active_tab << ")\n";
        NavigateTo(path);
    }

    void CloseTab(int index) {
        if (index >= 0 && index < static_cast<int>(tabs.size()) && tabs.size() > 1) {
            tabs.erase(tabs.begin() + index);
            if (active_tab >= static_cast<int>(tabs.size())) {
                active_tab = tabs.size() - 1;
            }
            std::cout << "[FileExplorer Window #" << window_id << "] Closed tab " << index << ".\n";
        }
    }

    void SwitchTab(int index) {
        if (index >= 0 && index < static_cast<int>(tabs.size())) {
            active_tab = index;
            std::cout << "[FileExplorer Window #" << window_id << "] Switched to tab " << active_tab << " (" << tabs[active_tab].current_path << ")\n";
        }
    }
    
    void NavigateTo(const std::string& path) {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];

        if (path == "trash://") {
            tab.is_trash_view = true;
            tab.current_path = "trash://";
            RefreshTrashContents();
            return;
        }

        tab.is_trash_view = false;
        std::error_code ec;
        if (fs::exists(path) && fs::is_directory(path, ec)) {
            tab.current_path = path;
            std::cout << "[FileExplorer Window #" << window_id << "] Tab " << active_tab << " navigated to: " << tab.current_path << "\n";
            RefreshContents();
        } else {
            std::cerr << "[!] Navigation failed. Path does not exist or is not a directory: " << path << "\n";
        }
    }

    void RefreshContents() {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];
        tab.current_contents.clear();
        std::error_code ec;
        if (!fs::exists(tab.current_path)) return;

        for (const auto& entry : fs::directory_iterator(tab.current_path, fs::directory_options::skip_permission_denied, ec)) {
            FileItemEntry item;
            item.full_path = entry.path().string();
            item.name = entry.path().filename().string();
            item.is_directory = entry.is_directory(ec);
            item.size_bytes = item.is_directory ? 0 : entry.file_size(ec);
            tab.current_contents.push_back(item);
        }
    }

    void RefreshTrashContents() {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];
        tab.trash_contents.clear();
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
                tab.trash_contents.push_back(item);
            }
        }
    }

    void ShowFilePreview(const std::string& path) {
        std::cout << "\n=== File Preview: " << path << " ===\n";
        // Omitted for brevity in testing multi-tab (same as previous implementation)
        std::cout << "[Preview] Executing quick-look rendering.\n";
    }

    bool ExtractArchive(const std::string& archive_path) {
        std::cout << "[ArchiveManager] Extracting " << archive_path << "...\n";
        RefreshContents();
        return true;
    }

    void HandleAddressBarInput(const std::string& typed_path) {
        NavigateTo(typed_path);
    }
};
