#include <iostream>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>

int main() {
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        std::cerr << "Error: Cannot open framebuffer device. Is gfxpayload set in GRUB?" << std::endl;
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    // Retrieve fixed and variable screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1 || 
        ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        std::cerr << "Error reading framebuffer information." << std::endl;
        close(fbfd);
        return 1;
    }

    long screensize = vinfo.yres_virtual * finfo.line_length;
    uint8_t* fbp = (uint8_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if (fbp == reinterpret_cast<uint8_t*>(-1)) {
        std::cerr << "Error: Failed to map framebuffer device to memory." << std::endl;
        close(fbfd);
        return 1;
    }

    // Draw a Nexus OS Gradient (Purple/Blue synthwave blend)
    for (int y = 0; y < vinfo.yres; y++) {
        for (int x = 0; x < vinfo.xres; x++) {
            long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                            (y + vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                fbp[location] = x % 255;            // Blue channel
                fbp[location + 1] = 50;             // Green channel
                fbp[location + 2] = y % 255;        // Red channel
                fbp[location + 3] = 0;              // Alpha channel
            } else if (vinfo.bits_per_pixel == 16) {
                // 5-6-5 RGB packing for 16-bit displays
                uint16_t b = (x % 255) >> 3;
                uint16_t g = 50 >> 2;
                uint16_t r = (y % 255) >> 3;
                *((uint16_t*)(fbp + location)) = (r << 11) | (g << 5) | b;
            }
        }
    }

    // Keep the splash screen visible for 3 seconds before continuing boot
    sleep(3);

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
