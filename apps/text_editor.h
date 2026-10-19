#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class TextEditorApp {
private:
    std::string current_filepath;
    std::vector<std::string> lines;
    int cursor_row = 0;
    int cursor_col = 0;
    bool is_modified = false;

public:
    TextEditorApp() {
        std::cout << "[TextEditor] Initializing native Text Editor application...\n";
        lines.push_back(""); // Start with one empty line
    }

    bool LoadFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[TextEditor] Failed to open file for reading: " << path << "\n";
            return false;
        }

        lines.clear();
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        if (lines.empty()) lines.push_back("");
        
        current_filepath = path;
        cursor_row = 0;
        cursor_col = 0;
        is_modified = false;
        
        std::cout << "[TextEditor] Successfully loaded " << path << " (" << lines.size() << " lines)\n";
        return true;
    }

    bool SaveFile(const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "[TextEditor] Failed to open file for writing: " << path << "\n";
            return false;
        }

        for (size_t i = 0; i < lines.size(); ++i) {
            file << lines[i];
            if (i < lines.size() - 1) file << "\n";
        }
        
        current_filepath = path;
        is_modified = false;
        
        std::cout << "[TextEditor] Successfully saved " << path << "\n";
        return true;
    }

    void HandleKeyboardEvent(int key_code, bool pressed) {
        if (!pressed) return;
        
        if (key_code == 8) { // Backspace
            if (cursor_col > 0) {
                lines[cursor_row].erase(cursor_col - 1, 1);
                cursor_col--;
                is_modified = true;
            } else if (cursor_row > 0) {
                // Merge with previous line
                cursor_col = lines[cursor_row - 1].length();
                lines[cursor_row - 1] += lines[cursor_row];
                lines.erase(lines.begin() + cursor_row);
                cursor_row--;
                is_modified = true;
            }
        } else if (key_code == 10 || key_code == 13) { // Enter/Return
            std::string remainder = lines[cursor_row].substr(cursor_col);
            lines[cursor_row] = lines[cursor_row].substr(0, cursor_col);
            lines.insert(lines.begin() + cursor_row + 1, remainder);
            cursor_row++;
            cursor_col = 0;
            is_modified = true;
        } else if (key_code >= 32 && key_code <= 126) { // Printable ASCII
            lines[cursor_row].insert(cursor_col, 1, static_cast<char>(key_code));
            cursor_col++;
            is_modified = true;
        }
        
        // Note: Arrow key navigation would be implemented here in a full system 
        // utilizing extended keycodes parsed from evdev.
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw Window Background
        gfx.FillRect(0, 0, 800, 600, 0xFFFFFFFF);
        
        // Draw Menu Bar
        gfx.FillRect(0, 0, 800, 30, 0xFFE0E0E0);
        gfx.DrawRect(0, 0, 800, 30, 0xFFCCCCCC, 1);
        font_engine.DrawText("File", 10, 20, 12, 0xFF000000);
        font_engine.DrawText("Edit", 50, 20, 12, 0xFF000000);
        font_engine.DrawText("View", 90, 20, 12, 0xFF000000);
        
        // Draw Status Bar
        gfx.FillRect(0, 580, 800, 20, 0xFFE0E0E0);
        std::string status = current_filepath.empty() ? "Untitled" : current_filepath;
        if (is_modified) status += " *";
        status += "  |  Ln " + std::to_string(cursor_row + 1) + ", Col " + std::to_string(cursor_col + 1);
        font_engine.DrawText(status, 10, 595, 12, 0xFF333333);

        // Draw Text Lines
        int start_y = 50;
        int line_height = 18;
        for (size_t i = 0; i < lines.size(); ++i) {
            font_engine.DrawText(lines[i], 10, start_y, 14, 0xFF000000);
            
            // Draw Cursor
            if (static_cast<int>(i) == cursor_row) {
                int cursor_x = 10 + (cursor_col * 8); // Approximation based on monospace font width
                gfx.FillRect(cursor_x, start_y - 12, 2, 14, 0xFF000000);
            }
            start_y += line_height;
            if (start_y > 570) break; // Prevent rendering over status bar
        }
    }
};
