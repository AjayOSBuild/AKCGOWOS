#pragma once
#include "../core/ui_toolkit.h"
#include "../core/file_metadata.h"
#include "../core/thumbnail_generator.h"
#include <iostream>
#include <string>
#include <algorithm>

class ImageViewerApp {
private:
    std::string current_image_path;
    int image_width = 0;
    int image_height = 0;
    double zoom_level = 1.0;
    int pan_x = 0;
    int pan_y = 0;
    FileMetadata metadata;

public:
    ImageViewerApp() {
        std::cout << "[ImageViewer] Initializing native Image Viewer application...\n";
    }

    bool LoadImage(const std::string& path) {
        if (!MetadataExtractor::GetMetadata(path, metadata)) {
            std::cerr << "[ImageViewer] Failed to read image metadata for: " << path << "\n";
            return false;
        }
        current_image_path = path;
        image_width = 1920; // Mock dimensions for scaffold
        image_height = 1080;
        zoom_level = 1.0;
        pan_x = 0;
        pan_y = 0;
        std::cout << "[ImageViewer] Loaded image: " << path << " (" << image_width << "x" << image_height << ")\n";
        return true;
    }

    void ZoomIn() {
        zoom_level *= 1.25;
        std::cout << "[ImageViewer] Zoom level: " << zoom_level * 100 << "%\n";
    }

    void ZoomOut() {
        zoom_level = std::max(0.1, zoom_level / 1.25);
        std::cout << "[ImageViewer] Zoom level: " << zoom_level * 100 << "%\n";
    }

    void ResetZoom() {
        zoom_level = 1.0;
        pan_x = 0;
        pan_y = 0;
        std::cout << "[ImageViewer] Zoom reset to 100%\n";
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Background (Dark photo-viewer theme)
        gfx.FillRect(0, 0, 800, 600, 0xFF111111);

        if (current_image_path.empty()) {
            font_engine.DrawText("No Image Loaded", 320, 280, 14, 0xFF888888);
            return;
        }

        // Render Image Viewport
        int draw_w = static_cast<int>(image_width * zoom_level);
        int draw_h = static_cast<int>(image_height * zoom_level);
        int draw_x = (800 - draw_w) / 2 + pan_x;
        int draw_y = (600 - draw_h) / 2 + pan_y;

        gfx.FillRect(draw_x, draw_y, draw_w, draw_h, 0xFF333333); // Placeholder canvas
        gfx.DrawRect(draw_x, draw_y, draw_w, draw_h, 0xFF555555, 1);
        font_engine.DrawText("[Image Render Stream]", draw_x + (draw_w / 2) - 60, draw_y + (draw_h / 2), 12, 0xFFCCCCCC);

        // Top Info Bar Overlay
        gfx.FillRect(0, 0, 800, 35, 0xCC222222);
        std::string info = metadata.path + "  (" + std::to_string(image_width) + "x" + std::to_string(image_height) + ")";
        font_engine.DrawText(info, 15, 22, 12, 0xFFFFFFFF);
    }
};
