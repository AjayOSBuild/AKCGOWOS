#pragma once
#include <string>

enum class SlotState {
    ACTIVE,
    STANDBY,
    UNBOOTABLE
};

struct SlotInfo {
    std::string name;       // "A" or "B"
    std::string device_path;// e.g., "/dev/vda2" or "/dev/vda3"
    SlotState state;
    int priority;           // Higher priority boots first
    int tries_remaining;    // Boot retry countdown before fallback
};

class AbSlotManager {
private:
    SlotInfo slot_a;
    SlotInfo slot_b;

    std::string GetActiveSlotName() const;

public:
    AbSlotManager();

    // Prints current A/B slot status table
    void PrintSlotStatus() const;

    // Writes the update payload to the inactive standby slot
    bool WriteToStandbySlot(const std::string& update_archive_path);

    // Atomically marks the standby slot as active and downgrades the current active slot
    bool PerformSlotSwitch();

    // Simulates a boot failure rollback mechanism
    bool RollbackIfNecessary();
};
