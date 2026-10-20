#include "journal_recovery.h"
#include <fstream>
#include <algorithm>

JournalEngine::JournalEngine(const std::string& journal_path) : journal_file_path(journal_path) {
    std::cout << "[Journal] Initializing Filesystem Journaling Recovery Engine...\n";
}

uint64_t JournalEngine::BeginTransaction() {
    current_tx_id++;
    transaction_states[current_tx_id] = TransactionState::STARTED;
    std::cout << "[Journal] Started Transaction TX_" << current_tx_id << "\n";
    return current_tx_id;
}

bool JournalEngine::RecordIntent(uint64_t tx_id, const std::string& op, const std::string& block, const std::string& old_data, const std::string& new_data) {
    if (transaction_states[tx_id] != TransactionState::STARTED) {
        std::cerr << "  -> [!] Cannot record intent for inactive transaction TX_" << tx_id << "\n";
        return false;
    }
    
    LogEntry entry = {tx_id, op, block, old_data, new_data};
    active_transactions[tx_id].push_back(entry);
    
    // Simulate sync to journal block device
    std::cout << "  -> Recorded: [" << op << "] on Block " << block << " (TX_" << tx_id << ")\n";
    return true;
}

bool JournalEngine::CommitTransaction(uint64_t tx_id) {
    if (transaction_states[tx_id] != TransactionState::STARTED) return false;
    
    transaction_states[tx_id] = TransactionState::COMMITTED;
    std::cout << "[Journal] Committed Transaction TX_" << tx_id << " to stable storage.\n";
    return true;
}

bool JournalEngine::RollbackTransaction(uint64_t tx_id) {
    if (transaction_states[tx_id] != TransactionState::STARTED) return false;
    
    transaction_states[tx_id] = TransactionState::ABORTED;
    active_transactions.erase(tx_id);
    std::cout << "[Journal] Aborted and rolled back Transaction TX_" << tx_id << ".\n";
    return true;
}

void JournalEngine::RecoverFilesystem() {
    std::cout << "\n[Journal] Starting Crash Recovery Sequence...\n";
    int recovered = 0;
    int rolled_back = 0;

    for (const auto& pair : transaction_states) {
        uint64_t tx_id = pair.first;
        TransactionState state = pair.second;

        if (state == TransactionState::COMMITTED) {
            std::cout << "  -> Replaying COMMITTED Transaction TX_" << tx_id << " (Ensuring writes hit disk)...\n";
            recovered++;
        } else if (state == TransactionState::STARTED) {
            std::cout << "  -> Cleaning up INCOMPLETE Transaction TX_" << tx_id << " (Torn write detected)...\n";
            rolled_back++;
        }
    }
    
    std::cout << "[+] Recovery Complete. Replayed: " << recovered << " | Rolled back: " << rolled_back << "\n";
    
    // Clear active states post-recovery
    active_transactions.clear();
    transaction_states.clear();
}

int main() {
    std::cout << "=== OS Project Filesystem Journal Utility ===\n";
    JournalEngine journal;

    std::cout << "\n--- Normal Operations ---\n";
    uint64_t tx1 = journal.BeginTransaction();
    journal.RecordIntent(tx1, "WRITE_INODE", "0x1A2B", "size:0", "size:4096");
    journal.RecordIntent(tx1, "ALLOC_BLOCK", "0x8F00", "free", "used");
    journal.CommitTransaction(tx1);

    std::cout << "\n--- Simulating Power Loss Mid-Transaction ---\n";
    uint64_t tx2 = journal.BeginTransaction();
    journal.RecordIntent(tx2, "WRITE_INODE", "0x3C4D", "size:1024", "size:2048");
    // Power loss occurs before journal.CommitTransaction(tx2)
    
    std::cout << "\n--- System Reboot ---\n";
    journal.RecoverFilesystem();

    std::cout << "\n[+] Journal Recovery Engine testing completed successfully.\n";
    return 0;
}
