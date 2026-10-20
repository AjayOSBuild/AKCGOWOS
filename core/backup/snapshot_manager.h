#pragma once
#include <string>
#include <map>
#include <vector>

namespace NexusOS {
namespace Backup {

struct Snapshot {
    int snapshot_id;
    std::string timestamp;
    int files_backed_up;
    int total_size_mb;
};

class SnapshotManager {
private:
    std::map<int, Snapshot> snapshots;
    int next_snapshot_id = 1;
    bool backup_enabled = false;
    std::string backup_destination = "";

public:
    void EnableBackup(const std::string& destination) {
        backup_enabled = true;
        backup_destination = destination;
    }

    void DisableBackup() {
        backup_enabled = false;
    }

    bool IsBackupEnabled() const { return backup_enabled; }
    std::string GetDestination() const { return backup_destination; }

    int CreateSnapshot(const std::string& timestamp, int files_changed, int size_mb) {
        if (!backup_enabled || files_changed <= 0) return -1;
        
        int id = next_snapshot_id++;
        snapshots[id] = {id, timestamp, files_changed, size_mb};
        return id;
    }

    bool RestoreSnapshot(int snapshot_id) {
        // In a real implementation, this would trigger file restorations from the backup drive
        return snapshots.find(snapshot_id) != snapshots.end();
    }
    
    int GetTotalSnapshots() const {
        return snapshots.size();
    }
};

} // namespace Backup
} // namespace NexusOS
