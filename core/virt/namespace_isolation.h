#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>

struct NamespaceConfig {
    bool isolate_pid = true;
    bool isolate_mount = true;
    bool isolate_net = true;
    bool isolate_ipc = true;
    bool isolate_uts = true;
    bool isolate_user = false;
};

class NamespaceIsolationManager {
public:
    NamespaceIsolationManager();

    // Computes the combined bitmask flags for linux namespace isolation
    int GetNamespaceFlags(const NamespaceConfig& config) const;

    // Applies namespace unsharing to isolate the current execution context
    bool UnshareNamespaces(const NamespaceConfig& config);
};
