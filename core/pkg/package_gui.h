#pragma once
#include "../../core/ui_toolkit.h"
#include "package_metadata.h"
#include "repository_format.h"
#include <iostream>
#include <string>
#include <vector>

class PackageManagerGUI {
private:
    PackageRegistry registry;
    RepositoryManager repo;
    std::vector<RemotePackageInfo> search_results;
    int selected_index = 0;
    
    enum class ViewState { BROWSER, DETAILS, INSTALLING };
    ViewState current_state = ViewState::BROWSER;
    
    std::string active_package;

public:
    PackageManagerGUI() {
        std::cout << "[PkgGUI] Initializing Nexus App Store Interface...\n";
        // In full execution, repo synchronization runs asynchronously.
        repo.UpdateIndexes();
        search_results = repo.Search(""); // Load all available by default
    }

    void HandleKeyboardEvent(int key_code, bool pressed) {
        if (!pressed) return;
        
        // Mock navigation mapping
        if (key_code == 80) { // Mock down arrow
            if (selected_index < static_cast<int>(search_results.size()) - 1) selected_index++;
        } else if (key_code == 72) { // Mock up arrow
            if (selected_index > 0) selected_index--;
        } else if (key_code == 10 || key_code == 13) { // Enter key
            if (current_state == ViewState::BROWSER && !search_results.empty()) {
                active_package = search_results[selected_index].meta.name;
                current_state = ViewState::DETAILS;
                std::cout << "[PkgGUI] Navigated to details view for: " << active_package << "\n";
            } else if (current_state == ViewState::DETAILS) {
                current_state = ViewState::INSTALLING;
                std::cout << "[PkgGUI] Triggered installation for: " << active_package << "...\n";
            }
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Main Background
        gfx.FillRect(0, 0, 900, 700, 0xFFFAFAFA);
        
        // App Header
        gfx.FillRect(0, 0, 900, 70, 0xFF1976D2); // Material Blue
        font_engine.DrawText("Nexus OS App Store", 20, 25, 24, 0xFFFFFFFF);
        
        if (current_state == ViewState::BROWSER) {
            font_engine.DrawText("Featured Applications", 20, 100, 18, 0xFF333333);
            
            int y_pos = 140;
            for (size_t i = 0; i < search_results.size(); ++i) {
                uint32_t text_color = (i == selected_index) ? 0xFF1976D2 : 0xFF555555;
                std::string prefix = (i == selected_index) ? "> " : "  ";
                font_engine.DrawText(prefix + search_results[i].meta.name + " (v" + search_results[i].meta.version + ")", 30, y_pos, 16, text_color);
                y_pos += 40;
            }
        } else if (current_state == ViewState::DETAILS) {
            font_engine.DrawText(active_package, 30, 100, 28, 0xFF222222);
            font_engine.DrawText("Official Nexus OS package.", 30, 140, 14, 0xFF666666);
            
            // Install Button
            gfx.FillRect(30, 180, 140, 45, 0xFF4CAF50); // Green install button
            font_engine.DrawText("INSTALL NOW", 50, 195, 14, 0xFFFFFFFF);
        } else if (current_state == ViewState::INSTALLING) {
            font_engine.DrawText("Installing " + active_package + "...", 30, 100, 24, 0xFF333333);
            gfx.DrawRect(30, 150, 400, 20, 0xFFCCCCCC, 2);
            gfx.FillRect(32, 152, 200, 16, 0xFF2196F3); // Simulated 50% progress bar
        }
    }
};
