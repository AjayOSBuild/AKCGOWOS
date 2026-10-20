# 1. Commit your local workspace changes to clear the working tree
git add Makefile s.sh core/archive_manager.cpp installer/iso_staging/boot/vmlinuz osprompt.md
git commit -m "chore: save local workspace modifications and tracking files"

# 2. Create and checkout the new feature branch
git checkout -b feature/DM-002-greeter-ui

# 3. Create the Greeter UI state machine logic
cat << 'EOF' > core/display_manager/greeter_ui.h
#pragma once
#include <string>

namespace NexusOS {
namespace DisplayManager {

enum class GreeterState {
    USER_SELECTION,
    PASSWORD_ENTRY,
    AUTHENTICATING,
    LOGIN_SUCCESS,
    LOGIN_FAILED
};

class GreeterUI {
private:
    GreeterState current_state = GreeterState::USER_SELECTION;
    std::string selected_user = "";

public:
    GreeterState GetState() const { return current_state; }
    std::string GetSelectedUser() const { return selected_user; }

    void SelectUser(const std::string& username) {
        selected_user = username;
        current_state = GreeterState::PASSWORD_ENTRY;
    }

    void CancelLogin() {
        selected_user = "";
        current_state = GreeterState::USER_SELECTION;
    }

    void SubmitCredentials(bool auth_success) {
        current_state = GreeterState::AUTHENTICATING;
        // Simulating immediate resolution for the state machine
        if (auth_success) {
            current_state = GreeterState::LOGIN_SUCCESS;
        } else {
            current_state = GreeterState::LOGIN_FAILED;
        }
    }

    void AcknowledgeFailure() {
        if (current_state == GreeterState::LOGIN_FAILED) {
            current_state = GreeterState::PASSWORD_ENTRY;
        }
    }
};

} // namespace DisplayManager
} // namespace NexusOS
EOF

# 4. Create the Unit Tests for the Greeter UI
cat << 'EOF' > tests/unit/greeter_test.cpp
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
EOF

# 5. Update the global orchestrator to include the new Greeter test
sed -i '/g++ -Wall -Wextra -std=c++17 unit\/display_manager_test.cpp/a g++ -Wall -Wextra -std=c++17 unit\/greeter_test.cpp -o "$BUILD_DIR\/greeter_test"' tests/run_all_tests.sh
sed -i '/"$BUILD_DIR\/display_manager_test"/a "$BUILD_DIR\/greeter_test"' tests/run_all_tests.sh

# 6. Build, Test, and Commit
cd tests
./run_all_tests.sh
cd ..
git add core/display_manager/greeter_ui.h tests/unit/greeter_test.cpp tests/run_all_tests.sh
git commit -m "DM-002: Implement Display Manager graphical greeter UI state machine and tests"