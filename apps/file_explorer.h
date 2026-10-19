#pragma once
#include "../core/ui_toolkit.h"
#include "../core/file_indexer.h"
#include "../core/file_metadata.h"
#include <memory>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct FileItemEntry {
    std::string name;
    std::string full_path;
    bool is_directory;
    uint64_t size_bytes;
};

class FileExplorerApp {
private:
    std::string current_path;
    FileIndexer indexer;
    std::vector<FileItemEntry> current_contents;
    
public:
    FileExplorerApp(const std::string& initial_path = "/mnt/usb") {
        NavigateTo(initial_path);
    }
    
    void NavigateTo(const std::string& path) {
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
    
    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Render split-pane layout: Sidebar on left, File grid on right
        gfx.FillRect(0, 30, 200, 738, 0xFFE0E0E0);
        gfx.DrawRect(0, 30, 200, 738, 0xFF888888, 1);
        
        gfx.FillRect(200, 30, 824, 738, 0xFFFFFFFF);
        
        font_engine.DrawText("Quick Access", 10, 50, 12, 0xFF000000);
        font_engine.DrawText("Volumes (/mnt)", 10, 80, 12, 0xFF000000);

        // Draw breadcrumb path header
        font_engine.DrawText("Path: " + current_path, 210, 50, 12, 0xFF000000);

        // Draw file list items
        int start_y = 80;
        for (const auto& item : current_contents) {
            std::string prefix = item.is_directory ? "[DIR]  " : "[FILE] ";
            font_engine.DrawText(prefix + item.name, 210, start_y, 12, 0xFF000000);
            start_y += 20;
            if (start_y > 720) break; // Viewport limit
        }
    }
    
    void HandleSearch(const std::string& query) {
        auto results = indexer.Search(query);
        std::cout << "[FileExplorer] Search query '" << query << "' returned " << results.size() << " results.\n";
    }
};
