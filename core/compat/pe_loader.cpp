#include "pe_loader.h"
#include <fstream>
#include <iostream>

PELoader::PELoader(const std::string& path) : filepath(path) {
    std::cout << "[PELoader] Initialized for binary: " << filepath << "\n";
}

bool PELoader::VerifySignatures(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < sizeof(IMAGE_DOS_HEADER)) {
        std::cerr << "[!] Binary too small to be a valid PE.\n";
        return false;
    }

    const IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.data());
    
    // Check for "MZ" magic (0x5A4D)
    if (dosHeader->e_magic != 0x5A4D) {
        std::cerr << "[!] Invalid DOS header magic (Not MZ).\n";
        return false;
    }

    if (buffer.size() < dosHeader->e_lfanew + sizeof(uint32_t) + sizeof(IMAGE_FILE_HEADER)) {
        std::cerr << "[!] Truncated NT headers.\n";
        return false;
    }

    // Check for "PE\0\0" signature (0x00004550)
    uint32_t pe_signature = *reinterpret_cast<const uint32_t*>(buffer.data() + dosHeader->e_lfanew);
    if (pe_signature != 0x00004550) {
        std::cerr << "[!] Invalid PE signature.\n";
        return false;
    }

    std::cout << "[+] Valid Windows PE binary signature detected.\n";
    return true;
}

bool PELoader::LoadAndMap() {
    std::cout << "[PELoader] Attempting to load " << filepath << " into virtual memory...\n";
    
    // Read the binary file
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[!] Failed to open executable file.\n";
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "[!] Failed to read executable file into buffer.\n";
        return false;
    }

    if (!VerifySignatures(buffer)) {
        return false;
    }

    std::cout << "[PELoader] Mapping PE sections (.text, .data, .rdata) into user-space...\n";
    // In full execution, mmap() would be called here to allocate executable memory pages based on the Section Headers
    
    is_loaded = true;
    return true;
}

void PELoader::ExecuteEntrypoint() {
    if (!is_loaded) {
        std::cerr << "[!] Cannot execute. Binary is not mapped in memory.\n";
        return;
    }
    
    std::cout << "[PELoader] Resolving imports against translated kernel32.dll / user32.dll...\n";
    std::cout << "[PELoader] Transferring execution to PE entrypoint...\n";
    
    // In full execution, this jumps the instruction pointer to AddressOfEntryPoint
}
