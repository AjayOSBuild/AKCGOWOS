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
#include <thread>
#include <atomic>
#include <functional>

namespace fs = std::filesystem;

// --- EXP: Large Operations Definitions ---
enum class ErrorAction { RETRY, SKIP, ABORT };

class FileOperationWorker {
public:
    std::atomic<uint64_t> total_bytes{0};
    std::atomic<uint64_t> copied_bytes{0};
    std::atomic<bool> is_running{false};
    std::atomic<bool> cancel_requested{false};
    std::string current_file;

    // Callbacks for UI integration
    std::function<ErrorAction(const std::string&, const std::string&)> on_error;
    std::function<void(bool)> on_complete;

    void StartCopy(const std::string& src, const std::string& dest) {
        if (is_running) return;
        is_running = true;
        cancel_requested = false;
        copied_bytes = 0;

        std::thread([this, src, dest]() {
            bool success = DoChunkedCopy(src, dest);
            is_running = false;
            if (on_complete) on_complete(success && !cancel_requested);
        }).detach();
    }

    void Cancel() {
        if (is_running) {
            std::cout << "[Worker] Cancellation requested by user...\n";
            cancel_requested = true;
        }
    }

private:
    bool DoChunkedCopy(const std::string& src, const std::string& dest) {
        std::error_code ec;
        if (!fs::exists(src, ec)) return false;
        
        total_bytes = fs::file_size(src, ec);
        current_file = fs::path(src).filename().string();

        while (true) {
            if (cancel_requested) return false;

            std::ifstream source(src, std::ios::binary);
            std::ofstream target(dest, std::ios::binary);

            if (!source.is_open() || !target.is_open()) {
                ErrorAction action = ErrorAction::ABORT;
                if (on_error) action = on_error(src, "Permission denied or file locked.");
                
                if (action == ErrorAction::RETRY) continue;
                if (action == ErrorAction::SKIP) return true;
                if (action == ErrorAction::ABORT) return false;
            }

            const size_t buffer_size = 65536; // 64 KB chunks for responsive cancellation
            std::vector<char> buffer(buffer_size);

            while (source.good()) {
                if (cancel_requested) {
                    source.close();
                    target.close();
                    fs::remove(dest, ec); // Cleanup partial file
                    return false;
                }

                source.read(buffer.data(), buffer_size);
                std::streamsize bytes_read = source.gcount();
                if (bytes_read > 0) {
                    target.write(buffer.data(), bytes_read);
                    copied_bytes += bytes_read;
                }
                
                // Simulate I/O latency for testing progress tracking
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            return true;
        }
    }
};

// --- Standard Explorer Definitions ---
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
    std::shared_ptr<FileOperationWorker> current_worker;

    FileExplorerApp(int win_id = 1, const std::string& initial_path = "/mnt/usb") : window_id(win_id) {
        OpenNewTab(initial_path);
    }

    void OpenNewTab(const std::string& path) {
        ExplorerTab new_tab;
        tabs.push_back(new_tab);
        active_tab = tabs.size() - 1;
        NavigateTo(path);
    }
    
    void NavigateTo(const std::string& path) {
        if (tabs.empty()) return;
        ExplorerTab& tab = tabs[active_tab];
        tab.is_trash_view = false;
        std::error_code ec;
        if (fs::exists(path) && fs::is_directory(path, ec)) {
            tab.current_path = path;
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
            FileItemEntry item;
            item.full_path = entry.path().string();
            item.name = entry.path().filename().string();
            item.is_directory = entry.is_directory(ec);
            item.size_bytes = item.is_directory ? 0 : entry.file_size(ec);
            tab.current_contents.push_back(item);
        }
    }

    // EXP: Large Operations Implementation
    void AsyncCopyItem(const std::string& source, const std::string& destination) {
        current_worker = std::make_shared<FileOperationWorker>();
        
        current_worker->on_error = [](const std::string& file, const std::string& reason) -> ErrorAction {
            std::cerr << "[UI Prompt] Error copying " << file << ": " << reason << "\n";
            std::cerr << "Options: [R]etry, [S]kip, [A]bort -> Auto-selecting ABORT\n";
            return ErrorAction::ABORT;
        };

        current_worker->on_complete = [this](bool success) {
            std::cout << "[FileExplorer] Async operation finished. Success: " << (success ? "YES" : "NO") << "\n";
            RefreshContents();
        };

        std::cout << "[FileExplorer] Starting async copy of " << source << " to " << destination << "...\n";
        current_worker->StartCopy(source, destination);
    }
};
