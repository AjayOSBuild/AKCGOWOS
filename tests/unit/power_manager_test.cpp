#include "../test_framework.h"
#include "../../core/power/power_manager.h"

using namespace NexusOS::Power;

TEST(PM_NormalTransitions) {
    PowerManager pm;
    ASSERT_EQ(static_cast<int>(ACPIState::S0_WORKING), static_cast<int>(pm.GetState()));
    
    // Test Sleep
    ASSERT_TRUE(pm.RequestStateTransition(ACPIState::S3_SLEEP));
    ASSERT_EQ(static_cast<int>(ACPIState::S3_SLEEP), static_cast<int>(pm.GetState()));
    
    // Test Wake
    ASSERT_TRUE(pm.WakeUp());
    ASSERT_EQ(static_cast<int>(ACPIState::S0_WORKING), static_cast<int>(pm.GetState()));
}

TEST(PM_CriticalBatteryFailsafe) {
    PowerManager pm;
    pm.UpdateBatteryStatus(3, false); // 3% battery, on battery power
    
    // Attempting to sleep on critical battery should fail and trigger shutdown
    ASSERT_EQ(false, pm.RequestStateTransition(ACPIState::S3_SLEEP));
    ASSERT_EQ(static_cast<int>(ACPIState::S5_SHUTDOWN), static_cast<int>(pm.GetState()));
}

int main() {
    return TestRegistry::RunAll();
}
