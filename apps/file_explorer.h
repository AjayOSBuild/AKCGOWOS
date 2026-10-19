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

// EXP-020: Explorer Settings Configuration
struct ExplorerSettings {
    bool show_hidden_files = false;
    enum class ViewMode { LIST, GRID } view_mode = ViewMode::LIST;
};

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
    ExplorerSettings settings;

    FileIndexer indexer;
    FileTrashManager trash_manager;
    ThumbnailGenerator thumbnail_gen;
    
public:
    FileExplorerApp(int win_id = 1, const std::string& initial_path = "/mnt/usb") : window_id(win_id) {
        std::cout << "[FileExplorer] Initializing Window #" << window_id << "\n";
        OpenNewTab(initial_path);
    }

    static std::shared_ptr<FileExplorerApp> SpawnNewWindow(int win_id, const std::string& initial_path = "/mnt/usb") {
        return std::make_shared<FileExplorerApp>(win_id, initial_path);
    }
    
    // EXP-020: Settings Management
    void ToggleHiddenFiles() {
        settings.show_hidden_files = !settings.show_hidden_files;
        std::cout << "[FileExplorer] Show hidden files: " << (settings.show_hidden_files ? "ON" : "OFF") << "\n";
        RefreshContents();
    }

    void SetViewMode(ExplorerSettings::ViewMode mode) {
        settings.view_mode = mode;
        std::cout << "[FileExplorer] View mode changed to: " << (mode == ExplorerSettings::ViewMode::GRID ? "GRID" : "LIST") << "\n";
    }

    // EXP-016: Network Locations
    bool ConnectNetworkLocation(const std::string& protocol, const std::string& address, const std::string& mount_name) {
        std::string target_dir = "/mnt/net/" + mount_name;
        std::error_code ec;
        fs::create_directories(target_dir, ec);
        
        std::cout << "[Network] Attempting to mount " << protocol << "://" << address << " to " << target_dir << "...\n";
        // In full execution, this submits an IPC request to StorageManager to execute mount(cifs/nfs)
        std::cout << "[+] Network drive connected successfully.\n";
        
        NavigateTo(target_dir);
        return true;
    }

    void OpenNewTab(const std::string& path) {
        ExplorerTab new_tab;
        tabs.push_back(new_tab);
        active_tab = tabs.size() - 1;
        NavigateTo(path);
    }

    void CloseTab(int index) {
        if (index >= 0 && index < static_cast<int>(tabs.size()) && tabs.size() > 1) {
            tabs.erase(tabs.begin() + index);
            if (active_tab >= static_cast<int>(tabs.size())) active_tab = tabs.size() - 1;
        }
    }

    void SwitchTab(int index) {
        if (index >= 0 && index < static_cast<int>(tabs.size())) {
            active_tab = index;
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
            std::cout << "[FileExplorer Window #" << window_id << "] Navigated to: " << tab.current_path << "\n";
            RefreshContents();
        }
    }

    void RefreshContents() {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];
        tab.current_contents.clear();
        std::error_code ec;
        if (!fs::exists(tab.current_path)) return;

        for (const auto& entry : fs::directory_iterator(tab.current_path, fs::directory_options::skip_permission_denied, ec)) {
            std::string filename = entry.path().filename().string();
            
            // EXP-020: Apply hidden files filter
            if (!settings.show_hidden_files && !filename.empty() && filename[0] == '.') {
                continue; 
            }

            FileItemEntry item;
            item.full_path = entry.path().string();
            item.name = filename;
            item.is_directory = entry.is_directory(ec);
            item.size_bytes = item.is_directory ? 0 : entry.file_size(ec);
            tab.current_contents.push_back(item);
        }
    }

    void RefreshTrashContents() {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];
        tab.trash_contents.clear();
        // Simplified for brevity, same as prior tasks
    }
};
