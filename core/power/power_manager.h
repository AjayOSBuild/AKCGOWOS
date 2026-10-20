#pragma once
#include <string>

namespace NexusOS {
namespace Power {

enum class ACPIState {
    S0_WORKING,
    S3_SLEEP,
    S4_HIBERNATE,
    S5_SHUTDOWN
};

class PowerManager {
private:
    ACPIState current_state = ACPIState::S0_WORKING;
    int battery_level = 100; // 0-100 percentage
    bool is_plugged_in = true;

public:
    ACPIState GetState() const { return current_state; }
    void UpdateBatteryStatus(int level, bool plugged_in) {
        battery_level = level < 0 ? 0 : (level > 100 ? 100 : level);
        is_plugged_in = plugged_in;
    }

    bool RequestStateTransition(ACPIState target_state) {
        // Prevent sleep or hibernate if battery is critically low and not plugged in
        if (!is_plugged_in && battery_level < 5 && 
           (target_state == ACPIState::S3_SLEEP || target_state == ACPIState::S4_HIBERNATE)) {
            // Force shutdown instead of sleeping to prevent data loss
            current_state = ACPIState::S5_SHUTDOWN;
            return false; 
        }

        // Valid transition
        current_state = target_state;
        return true;
    }

    bool WakeUp() {
        if (current_state == ACPIState::S3_SLEEP || current_state == ACPIState::S4_HIBERNATE) {
            current_state = ACPIState::S0_WORKING;
            return true;
        }
        return false;
    }
};

} // namespace Power
} // namespace NexusOS
