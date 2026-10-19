#pragma once
#include <iostream>
#include <string>
#include <vector>

struct AndroidGraphicBuffer {
    int buffer_id;
    int width;
    int height;
    int stride;
    int format;
    int host_memfd; // The shared memory file descriptor on the Nexus OS host
};

class AndroidGraphicsBridge {
private:
    std::string display_socket_path;
    int next_buffer_id = 1;
    std::vector<AndroidGraphicBuffer> active_buffers;

    int CreateHostMemfd(size_t size);

public:
    AndroidGraphicsBridge(const std::string& ipc_socket = "/run/os/display.sock");
    ~AndroidGraphicsBridge();

    // Intercepts Android gralloc/SurfaceFlinger allocation requests
    AndroidGraphicBuffer* AllocateBuffer(int width, int height, int format);

    // Submits the rendered buffer to the Nexus OS Compositor
    bool SubmitBuffer(int buffer_id);
    
    // Releases the buffer mapping
    void FreeBuffer(int buffer_id);
};
