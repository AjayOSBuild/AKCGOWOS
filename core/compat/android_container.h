#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include "apk_parser.h"

class AndroidContainerManager {
private:
    std::string container_id;
    pid_t container_pid = -1;
    bool is_running = false;
    std::string rootfs_path;

    bool SetupNamespaces();
    bool SetupCgroups();
    bool MountRootfs();

public:
    AndroidContainerManager(const std::string& id);
    ~AndroidContainerManager();

    // Initializes the isolated environment and executes the parsed APK
    bool StartApp(const ApkInfo& app_info, const std::string& apk_path);
    
    // Safely tears down the container and cleans up mounts
    bool StopContainer();
    
    bool IsRunning() const { return is_running; }
};
