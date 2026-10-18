#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>

class FileWatcher {
private:
    int inotify_fd = -1;
    std::unordered_map<int, std::string> watch_descriptors;

public:
    FileWatcher() {
        inotify_fd = inotify_init1(IN_NONBLOCK);
        if (inotify_fd < 0) {
            std::cerr << "[!] Failed to initialize inotify file descriptor.\n";
        } else {
            std::cout << "[FileWatcher] inotify subsystem initialized successfully.\n";
        }
    }

    ~FileWatcher() {
        if (inotify_fd >= 0) {
            close(inotify_fd);
        }
    }

    int AddWatch(const std::string& path) {
        if (inotify_fd < 0) return -1;

        // Watch for file creation, deletion, modification, and moves
        int wd = inotify_add_watch(inotify_fd, path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);
        if (wd < 0) {
            std::cerr << "[!] Failed to add watch for path: " << path << "\n";
            return -1;
        }

        watch_descriptors[wd] = path;
        std::cout << "[FileWatcher] Added watch on path: " << path << " (WD: " << wd << ")\n";
        return wd;
    }

    void PollEvents(std::function<void(const std::string&, uint32_t)> callback) {
        if (inotify_fd < 0) return;

        char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
        ssize_t len = read(inotify_fd, buffer, sizeof(buffer));
        
        if (len < 0) {
            return; // No events or non-blocking read returned EAGAIN
        }

        const struct inotify_event *event;
        for (char *ptr = buffer; ptr < buffer + len; ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;
            if (event->len > 0) {
                auto it = watch_descriptors.find(event->wd);
                if (it != watch_descriptors.end()) {
                    std::string full_path = it->second + "/" + event->name;
                    callback(full_path, event->mask);
                }
            }
        }
    }
};
