#pragma once
#include "../core/ui_toolkit.h"
#include "../core/file_indexer.h"
#include "../core/file_metadata.h"
#include <memory>
#include <vector>

class FileExplorerApp {
private:
    std::string current_path;
    FileIndexer indexer;
    
public:
    FileExplorerApp(const std::string& initial_path = "/mnt/usb") : current_path(initial_path) {
        std::cout << "[FileExplorer] Initializing window for path: " << current_path << "\n";
        indexer.IndexDirectory(current_path);
    }
    
    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Render split-pane layout: Sidebar on left, File grid on right
        // Sidebar background
        gfx.FillRect(0, 30, 200, 738, 0xFFE0E0E0);
        gfx.DrawRect(0, 30, 200, 738, 0xFF888888, 1);
        
        // File Grid background
        gfx.FillRect(200, 30, 824, 738, 0xFFFFFFFF);
        
        font_engine.DrawText("Quick Access", 10, 50, 12, 0xFF000000);
        font_engine.DrawText("Volumes (/mnt)", 10, 80, 12, 0xFF000000);
    }
    
    void HandleSearch(const std::string& query) {
        auto results = indexer.Search(query);
        std::cout << "[FileExplorer] Search query '" << query << "' returned " << results.size() << " results.\n";
    }
};
