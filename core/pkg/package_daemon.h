#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>

enum class DaemonCommandType {
    SYNC_REPOS,
    INSTALL_PACKAGE,
    UNINSTALL_PACKAGE,
    CHECK_UPDATES,
    SHUTDOWN
};

struct DaemonRequest {
    DaemonCommandType type;
    std::string payload;
};

class PackageDaemon {
private:
    std::string ipc_socket_path;
    bool is_running = false;
    std::queue<DaemonRequest> request_queue;
    std::mutex queue_mutex;

public:
    PackageDaemon(const std::string& socket_path = "/run/os/pkg_daemon.sock");
    ~PackageDaemon();

    // Initializes Unix Domain Socket for client IPC requests
    bool InitializeListener();

    // Enqueues an asynchronous request from the GUI or CLI
    void SubmitRequest(const DaemonRequest& req);

    // Background worker loop processing queued package tasks
    void RunServiceLoop();

    // Gracefully stops the daemon service
    void Stop();
};
