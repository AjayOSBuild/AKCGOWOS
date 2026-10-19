#include "android_graphics_bridge.h"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

AndroidGraphicsBridge::AndroidGraphicsBridge(const std::string& ipc_socket) 
    : display_socket_path(ipc_socket) {
    std::cout << "[AndroidGraphicsBridge] Initialized. Targeting host display socket: " << display_socket_path << "\n";
}

AndroidGraphicsBridge::~AndroidGraphicsBridge() {
    std::cout << "[AndroidGraphicsBridge] Shutting down. Cleaning up shared memory buffers...\n";
    for (const auto& buf : active_buffers) {
        if (buf.host_memfd > 0) {
            close(buf.host_memfd);
        }
    }
}

int AndroidGraphicsBridge::CreateHostMemfd(size_t size) {
    // In full execution, this invokes memfd_create() syscall.
    // We mock the file descriptor return for scaffolding.
    std::cout << "[AndroidGraphicsBridge] Creating zero-copy memfd buffer of size " << size << " bytes.\n";
    return 1024 + next_buffer_id; // Mock FD
}

AndroidGraphicBuffer* AndroidGraphicsBridge::AllocateBuffer(int width, int height, int format) {
    std::cout << "[AndroidGraphicsBridge] Intercepted SurfaceFlinger allocation request (" 
              << width << "x" << height << ", format " << format << ").\n";
              
    // Standard RGBA8888 calculation
    size_t buffer_size = width * height * 4; 
    
    AndroidGraphicBuffer buf;
    buf.buffer_id = next_buffer_id++;
    buf.width = width;
    buf.height = height;
    buf.stride = width * 4;
    buf.format = format;
    buf.host_memfd = CreateHostMemfd(buffer_size);

    active_buffers.push_back(buf);
    
    std::cout << "  -> Mapped Android ashmem to host memfd: " << buf.host_memfd << "\n";
    return &active_buffers.back();
}

bool AndroidGraphicsBridge::SubmitBuffer(int buffer_id) {
    for (const auto& buf : active_buffers) {
        if (buf.buffer_id == buffer_id) {
            std::cout << "[AndroidGraphicsBridge] Committing buffer " << buffer_id 
                      << " (memfd: " << buf.host_memfd << ") to Nexus OS Compositor IPC...\n";
            // In full execution, this sends the FD over the Unix Domain Socket with SCM_RIGHTS.
            return true;
        }
    }
    std::cerr << "[!] Attempted to submit invalid buffer ID: " << buffer_id << "\n";
    return false;
}

void AndroidGraphicsBridge::FreeBuffer(int buffer_id) {
    for (auto it = active_buffers.begin(); it != active_buffers.end(); ++it) {
        if (it->buffer_id == buffer_id) {
            std::cout << "[AndroidGraphicsBridge] Releasing buffer " << buffer_id << " (memfd: " << it->host_memfd << ")\n";
            close(it->host_memfd);
            active_buffers.erase(it);
            return;
        }
    }
}
