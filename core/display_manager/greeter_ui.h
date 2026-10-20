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
