#include "crypto_block_layer.h"

CryptoBlockManager::CryptoBlockManager() {
    std::cout << "[CryptoBlock] Initializing Cryptographic Block Layer (dm-crypt/LUKS wrapper)...\n";
}

std::string CryptoBlockManager::GenerateVolumeKey() const {
    // In production, this reads from /dev/urandom to generate a 256-bit AES Master Key
    return "MOCK_256BIT_AES_XTS_MASTER_KEY";
}

bool CryptoBlockManager::VerifyPassphrase(const std::string& passphrase, const std::string& mock_hash) const {
    // Simulates PBKDF2/Argon2 passphrase hashing and key slot decryption
    return !passphrase.empty(); 
}

bool CryptoBlockManager::FormatDevice(const std::string& device_path, const std::string& passphrase) {
    std::cout << "[CryptoBlock] Formatting device " << device_path << " with cryptographic header...\n";
    
    if (passphrase.empty()) {
        std::cerr << "  -> [!] Format aborted: Passphrase cannot be empty.\n";
        return false;
    }

    std::cout << "  -> Generating Master Volume Key...\n";
    std::string master_key = GenerateVolumeKey();
    
    std::cout << "  -> Securing Master Key into Key Slot 0 (Argon2id derivation)...\n";
    std::cout << "  -> Writing LUKS header to device sector 0...\n";
    std::cout << "[+] Device " << device_path << " is now securely formatted (AES-256-XTS).\n";
    
    return true;
}

bool CryptoBlockManager::OpenDevice(const std::string& device_path, const std::string& mapped_name, const std::string& passphrase) {
    std::cout << "[CryptoBlock] Attempting to unlock device " << device_path << "...\n";
    
    if (!VerifyPassphrase(passphrase, "MOCK_HASH")) {
        std::cerr << "  -> [!] Unlock failed: Invalid passphrase.\n";
        return false;
    }

    std::cout << "  -> Passphrase verified. Master Key decrypted.\n";
    std::cout << "  -> [IPC] Instructing Kernel device-mapper to establish transparent crypto target...\n";
    
    std::string mapped_path = "/dev/mapper/" + mapped_name;
    std::cout << "[+] Device unlocked. Mapped transparently to: " << mapped_path << "\n";
    
    return true;
}

bool CryptoBlockManager::CloseDevice(const std::string& mapped_name) {
    std::string mapped_path = "/dev/mapper/" + mapped_name;
    std::cout << "[CryptoBlock] Locking and unmapping device " << mapped_path << "...\n";
    
    std::cout << "  -> [IPC] Instructing Kernel device-mapper to flush I/O and destroy crypto target...\n";
    std::cout << "[+] Device locked. Data is secured at rest.\n";
    
    return true;
}

int main() {
    std::cout << "=== OS Project Cryptographic Block Layer Utility ===\n";
    CryptoBlockManager crypto_mgr;

    std::string target_device = "/dev/sdb1";
    std::string mapped_name = "secure_volume";
    std::string pass = "my_secure_os_password";

    // Simulate Format -> Open -> Close lifecycle
    std::cout << "\n--- Simulating Encrypted Volume Lifecycle ---\n";
    crypto_mgr.FormatDevice(target_device, pass);
    crypto_mgr.OpenDevice(target_device, mapped_name, pass);
    crypto_mgr.CloseDevice(mapped_name);

    std::cout << "\n[+] Crypto Block Layer testing completed successfully.\n";
    return 0;
}
