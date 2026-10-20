#include "../test_framework.h"
#include "../../core/display_manager/greeter_ui.h"

using namespace NexusOS::DisplayManager;

TEST(DM_Greeter_UserSelection) {
    GreeterUI ui;
    ASSERT_EQ(static_cast<int>(GreeterState::USER_SELECTION), static_cast<int>(ui.GetState()));
    
    ui.SelectUser("admin");
    ASSERT_EQ("admin", ui.GetSelectedUser());
    ASSERT_EQ(static_cast<int>(GreeterState::PASSWORD_ENTRY), static_cast<int>(ui.GetState()));
}

TEST(DM_Greeter_AuthFlow) {
    GreeterUI ui;
    ui.SelectUser("guest");
    
    // Simulate failed login
    ui.SubmitCredentials(false);
    ASSERT_EQ(static_cast<int>(GreeterState::LOGIN_FAILED), static_cast<int>(ui.GetState()));
    
    // Reset to password entry after failure
    ui.AcknowledgeFailure();
    ASSERT_EQ(static_cast<int>(GreeterState::PASSWORD_ENTRY), static_cast<int>(ui.GetState()));
    
    // Simulate successful login
    ui.SubmitCredentials(true);
    ASSERT_EQ(static_cast<int>(GreeterState::LOGIN_SUCCESS), static_cast<int>(ui.GetState()));
}

int main() {
    return TestRegistry::RunAll();
}
