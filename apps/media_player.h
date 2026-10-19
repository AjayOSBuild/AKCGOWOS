#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>

enum class PlaybackState { STOPPED, PLAYING, PAUSED };

class MediaPlayerApp {
private:
    std::string current_media_path;
    PlaybackState state = PlaybackState::STOPPED;
    double current_time_sec = 0.0;
    double total_duration_sec = 0.0;

public:
    MediaPlayerApp() {
        std::cout << "[MediaPlayer] Initializing native Media Player application...\n";
    }

    bool LoadMedia(const std::string& path) {
        current_media_path = path;
        // In full execution, this invokes FFmpeg/GStreamer bindings to parse demux headers
        std::cout << "[MediaPlayer] Loading media file: " << path << "\n";
        
        // Mock duration
        total_duration_sec = 120.0; 
        current_time_sec = 0.0;
        state = PlaybackState::STOPPED;
        
        std::cout << "[MediaPlayer] Ready. Duration: 02:00\n";
        return true;
    }

    void Play() {
        if (current_media_path.empty()) return;
        state = PlaybackState::PLAYING;
        std::cout << "[MediaPlayer] Playback STARTED. Routing audio to ALSA backend...\n";
    }

    void Pause() {
        if (state == PlaybackState::PLAYING) {
            state = PlaybackState::PAUSED;
            std::cout << "[MediaPlayer] Playback PAUSED.\n";
        }
    }

    void Stop() {
        state = PlaybackState::STOPPED;
        current_time_sec = 0.0;
        std::cout << "[MediaPlayer] Playback STOPPED.\n";
    }

    void Seek(double time_sec) {
        if (time_sec >= 0.0 && time_sec <= total_duration_sec) {
            current_time_sec = time_sec;
            std::cout << "[MediaPlayer] Seeked to " << current_time_sec << "s\n";
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw Window Background
        gfx.FillRect(0, 0, 800, 600, 0xFF222222); // Dark theme for media
        
        // Draw Video Viewport (Letterboxed)
        gfx.FillRect(0, 0, 800, 500, 0xFF000000);
        if (state == PlaybackState::STOPPED) {
            font_engine.DrawText("Nexus Media Engine - No Signal", 300, 250, 14, 0xFF888888);
        } else {
            // In a real implementation, decoded frames are blitted here via zero-copy memfd
            font_engine.DrawText("[Video Frame Buffer]", 340, 250, 14, 0xFF555555);
        }

        // Draw Control Bar Background
        gfx.FillRect(0, 500, 800, 100, 0xFF333333);
        gfx.DrawRect(0, 500, 800, 100, 0xFF111111, 2);

        // Draw Timeline / Scrubber
        gfx.FillRect(50, 520, 700, 10, 0xFF555555); // Background track
        if (total_duration_sec > 0) {
            int progress_width = static_cast<int>((current_time_sec / total_duration_sec) * 700);
            gfx.FillRect(50, 520, progress_width, 10, 0xFF0088FF); // Progress fill
            gfx.DrawRect(50 + progress_width - 5, 515, 10, 20, 0xFFFFFFFF, 1); // Scrubber handle
        }

        // Draw Time Text
        std::string time_text = std::to_string(static_cast<int>(current_time_sec)) + "s / " + 
                                std::to_string(static_cast<int>(total_duration_sec)) + "s";
        font_engine.DrawText(time_text, 700, 545, 12, 0xFFCCCCCC);

        // Draw Transport Controls (Mocked buttons)
        int btn_y = 540;
        gfx.FillRect(350, btn_y, 40, 40, 0xFF555555); // Play/Pause
        font_engine.DrawText((state == PlaybackState::PLAYING ? "||" : ">"), 365, btn_y + 25, 16, 0xFFFFFFFF);

        gfx.FillRect(410, btn_y, 40, 40, 0xFF555555); // Stop
        font_engine.DrawText("[]", 423, btn_y + 25, 16, 0xFFFFFFFF);
        
        // Draw Media Title
        font_engine.DrawText(current_media_path.empty() ? "No Media Loaded" : current_media_path, 50, 555, 14, 0xFFFFFFFF);
    }
};
