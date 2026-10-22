#include <iostream>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>

long max_size = 0;

void draw_pixel(uint8_t* fbp, int x, int y, uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo& vinfo, struct fb_fix_screeninfo& finfo) {
    if (x < 0 || x >= (int)vinfo.xres || y < 0 || y >= (int)vinfo.yres) return;
    
    int bpp = vinfo.bits_per_pixel / 8;
    if (bpp == 0) return; 

    long location = (x + vinfo.xoffset) * bpp + (y + vinfo.yoffset) * finfo.line_length;
    
    // Bounds check to prevent Segfault crashes
    if (location < 0 || location > max_size - 4) return;

    if (vinfo.bits_per_pixel == 16) {
        uint16_t c = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((uint16_t*)(fbp + location)) = c;
    } else {
        if (bpp >= 1) fbp[location] = b;
        if (bpp >= 2) fbp[location + 1] = g;
        if (bpp >= 3) fbp[location + 2] = r;
    }
}

void draw_cursor(uint8_t* fbp, int cx, int cy, bool clear, struct fb_var_screeninfo& vinfo, struct fb_fix_screeninfo& finfo) {
    uint8_t color = clear ? 0 : 255; 
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 30; x++) {
            draw_pixel(fbp, cx + x, cy + y, color, color, color, vinfo, finfo);
        }
    }
}

int main() {
    int tty_fd = open("/dev/tty1", O_RDWR);
    if (tty_fd >= 0) ioctl(tty_fd, KDSETMODE, KD_GRAPHICS);

    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) return 1;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);

    long screensize = vinfo.yres_virtual * finfo.line_length;
    max_size = screensize;
    
    uint8_t* fbp = (uint8_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    for (long i = 0; i < screensize; i++) fbp[i] = 0;

    int box_x = 100, box_y = 100;
    int velocity_x = 6, velocity_y = 4;
    int box_size = 30;

    for (int frames = 0; frames < 600; frames++) {
        draw_cursor(fbp, box_x, box_y, true, vinfo, finfo);
        
        box_x += velocity_x;
        box_y += velocity_y;

        if (box_x <= 0 || box_x >= (int)vinfo.xres - box_size) {
            velocity_x = -velocity_x;
            box_x = (box_x <= 0) ? 0 : vinfo.xres - box_size;
        }
        if (box_y <= 0 || box_y >= (int)vinfo.yres - box_size) {
            velocity_y = -velocity_y;
            box_y = (box_y <= 0) ? 0 : vinfo.yres - box_size;
        }

        draw_cursor(fbp, box_x, box_y, false, vinfo, finfo);
        usleep(16000); 
    }

    munmap(fbp, screensize);
    close(fbfd);
    
    if (tty_fd >= 0) {
        ioctl(tty_fd, KDSETMODE, KD_TEXT);
        close(tty_fd);
    }
    return 0;
}
