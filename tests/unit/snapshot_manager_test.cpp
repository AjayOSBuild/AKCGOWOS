#include "../test_framework.h"
#include "../../core/backup/snapshot_manager.h"

using namespace NexusOS::Backup;

TEST(BAK_SnapshotCreation) {
    SnapshotManager sm;
    
    // Should fail if backup is not enabled
    ASSERT_EQ(-1, sm.CreateSnapshot("2026-10-20T10:00:00", 50, 120));
    
    sm.EnableBackup("/mnt/external_drive/NexusBackups");
    ASSERT_TRUE(sm.IsBackupEnabled());
    
    // Should succeed now
    int snap_id = sm.CreateSnapshot("2026-10-20T10:00:00", 50, 120);
    ASSERT_TRUE(snap_id > 0);
    ASSERT_EQ(1, sm.GetTotalSnapshots());
    
    // Should fail on 0 files changed
    ASSERT_EQ(-1, sm.CreateSnapshot("2026-10-21T10:00:00", 0, 0));
}

TEST(BAK_SnapshotRestoration) {
    SnapshotManager sm;
    sm.EnableBackup("/mnt/backup");
    
    int snap_id = sm.CreateSnapshot("2026-10-20T12:00:00", 100, 500);
    
    ASSERT_TRUE(sm.RestoreSnapshot(snap_id));
    ASSERT_EQ(false, sm.RestoreSnapshot(999)); // Invalid ID
}

int main() {
    return TestRegistry::RunAll();
}
