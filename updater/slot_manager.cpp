#include <iomanip>
#include "slot_manager.h"
#include <iostream>
#include <unistd.h>

AbSlotManager::AbSlotManager() {
    // Initialize mock state: Slot A is active, Slot B is standby
    slot_a = {"A", "/dev/vda2", SlotState::ACTIVE, 100, 3};
    slot_b = {"B", "/dev/vda3", SlotState::STANDBY, 50, 3};
    std::cout << "[AbSlotManager] Initializing transactional A/B slot management controller...\n";
}

std::string AbSlotManager::GetActiveSlotName() const {
    if (slot_a.state == SlotState::ACTIVE) return slot_a.name;
    if (slot_b.state == SlotState::ACTIVE) return slot_b.name;
    return "UNKNOWN";
}

void AbSlotManager::PrintSlotStatus() const {
    std::cout << "\n======================================================\n";
    std::cout << "             TRANSACTIONAL A/B SLOT STATUS            \n";
    std::cout << "======================================================\n";
    std::cout << " Slot [" << slot_a.name << "] path: " << std::setw(10) << slot_a.device_path 
              << " | State: " << (slot_a.state == SlotState::ACTIVE ? "ACTIVE" : (slot_a.state == SlotState::STANDBY ? "STANDBY" : "UNBOOTABLE"))
              << " | Priority: " << slot_a.priority << "\n";
    std::cout << " Slot [" << slot_b.name << "] path: " << std::setw(10) << slot_b.device_path 
              << " | State: " << (slot_b.state == SlotState::ACTIVE ? "ACTIVE" : (slot_b.state == SlotState::STANDBY ? "STANDBY" : "UNBOOTABLE"))
              << " | Priority: " << slot_b.priority << "\n";
    std::cout << "======================================================\n";
    std::cout << " Current Active Boot Slot: [" << GetActiveSlotName() << "]\n";
    std::cout << "======================================================\n";
}

bool AbSlotManager::WriteToStandbySlot(const std::string& update_archive_path) {
    SlotInfo& standby = (slot_a.state == SlotState::ACTIVE) ? slot_b : slot_a;
    
    std::cout << "[AbSlotManager] Writing update payload from '" << update_archive_path 
              << "' into inactive standby slot [" << standby.name << "] (" << standby.device_path << ")...\n";
    
    // Simulate block writing delay
    sleep(1);
    std::cout << "[+] Standby slot successfully flashed and synchronized.\n";
    return true;
}

bool AbSlotManager::PerformSlotSwitch() {
    SlotInfo& active = (slot_a.state == SlotState::ACTIVE) ? slot_a : slot_b;
    SlotInfo& standby = (slot_a.state == SlotState::ACTIVE) ? slot_b : slot_a;

    std::cout << "[AbSlotManager] Executing atomic transaction to swap boot slots...\n";
    std::cout << "  -> Demoting current active slot [" << active.name << "] to STANDBY\n";
    std::cout << "  -> Promoting standby slot [" << standby.name << "] to ACTIVE\n";

    // Swap states and priorities atomically
    SlotState temp_state = active.state;
    active.state = standby.state;
    standby.state = temp_state;

    int temp_prio = active.priority;
    active.priority = standby.priority;
    standby.priority = temp_prio + 10; // Ensure new active has highest priority

    std::cout << "[+] Atomic A/B slot swap completed successfully. Next reboot will load Slot [" << GetActiveSlotName() << "].\n";
    return true;
}

bool AbSlotManager::RollbackIfNecessary() {
    std::cout << "[AbSlotManager] Checking boot health metrics...\n";
    // Simulated rollback check
    return false; // No failure detected
}

int main() {
    std::cout << "=== OS Project Update System: A/B Slot Manager Utility ===\n";
    
    AbSlotManager slot_mgr;
    slot_mgr.PrintSlotStatus();

    std::cout << "\n[Test 1] Writing update to standby partition...\n";
    slot_mgr.WriteToStandbySlot("/tmp/nexus-os-update.tar.gz");

    std::cout << "\n[Test 2] Performing atomic slot switch...\n";
    slot_mgr.PerformSlotSwitch();
    
    slot_mgr.PrintSlotStatus();

    std::cout << "[+] A/B Slot Swapping testing completed successfully.\n";
    return 0;
}
