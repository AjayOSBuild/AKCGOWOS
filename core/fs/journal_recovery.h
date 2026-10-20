#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class TransactionState {
    STARTED,
    COMMITTED,
    ABORTED
};

struct LogEntry {
    uint64_t tx_id;
    std::string operation;
    std::string block_address;
    std::string old_data;
    std::string new_data;
};

class JournalEngine {
private:
    std::string journal_file_path;
    uint64_t current_tx_id = 0;
    std::unordered_map<uint64_t, std::vector<LogEntry>> active_transactions;
    std::unordered_map<uint64_t, TransactionState> transaction_states;

public:
    JournalEngine(const std::string& journal_path = "/var/log/nexus_fs_journal.log");

    // Starts a new atomic transaction
    uint64_t BeginTransaction();

    // Records a metadata change intent to the journal before modifying disk
    bool RecordIntent(uint64_t tx_id, const std::string& op, const std::string& block, const std::string& old_data, const std::string& new_data);

    // Marks the transaction as safely written to the journal
    bool CommitTransaction(uint64_t tx_id);

    // Discards the transaction (e.g., on failure before commit)
    bool RollbackTransaction(uint64_t tx_id);

    // Simulates mounting a dirty filesystem and replaying the journal
    void RecoverFilesystem();
};
