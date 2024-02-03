#include "../include/display.h"

#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys/ioctl.h"
#include "sys/mman.h"

byte_t display[MAX_DISP_ROW * MAX_DISP_COL] = {(byte_t)0x00};
byte_t sprites[MAX_SPRITES] = { (byte_t)0xF0, (byte_t)0x90, (byte_t)0x90, (byte_t)0x90, (byte_t)0xF0,
                                (byte_t)0x20, (byte_t)0x60, (byte_t)0x20, (byte_t)0x20, (byte_t)0x70,
                                (byte_t)0xF0, (byte_t)0x10, (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x10, (byte_t)0xF0, (byte_t)0x10, (byte_t)0xF0,
                                (byte_t)0x90, (byte_t)0x90, (byte_t)0xF0, (byte_t)0x10, (byte_t)0x10,
                                (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0, (byte_t)0x10, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0, (byte_t)0x90, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x10, (byte_t)0x20, (byte_t)0x40, (byte_t)0x40,
                                (byte_t)0xF0, (byte_t)0x90, (byte_t)0xF0, (byte_t)0x90, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x90, (byte_t)0xF0, (byte_t)0x10, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x90, (byte_t)0xF0, (byte_t)0x90, (byte_t)0x90,
                                (byte_t)0xE0, (byte_t)0x90, (byte_t)0xE0, (byte_t)0x90, (byte_t)0xE0,
                                (byte_t)0xF0, (byte_t)0x80, (byte_t)0x80, (byte_t)0x80, (byte_t)0xF0,
                                (byte_t)0xE0, (byte_t)0x90, (byte_t)0x90, (byte_t)0x90, (byte_t)0xE0,
                                (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0,
                                (byte_t)0xF0, (byte_t)0x80, (byte_t)0xF0, (byte_t)0x80, (byte_t)0x80};

struct fb_fix_screeninfo fbfs;
struct fb_var_screeninfo fbvs;
int fd;
unsigned long *start_mmap_fb;
unsigned long *screen_data;

void init_fb() {
    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1) {
        perror("Error opening /dev/fb0 in init_fb()");
        exit(1);
    }
    if (ioctl(fd, FBIOGET_VSCREENINFO, &fbvs) == -1) {
        perror("Error getting VSCREENINFO from /dev/fb0 in init_fb()");
        close_fb();
        exit(1);
    }
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fbfs) == -1) {
        perror("Error getting FSCREENINFO from /dev/fb0 in init_fb()");
        close_fb();
        exit(1);
    }
    start_mmap_fb = (unsigned long*)mmap(NULL, fbfs.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
    if (start_mmap_fb == (void*)-1) {
        perror("Error mapping /dev/fb0 to memory in init_fb()");
        close_fb();
        exit(1);
    }
    screen_data = start_mmap_fb + fbfs.smem_start;
    printf("Screen dimensions: %d x %d\n", fbvs.xres, fbvs.yres);
}

void close_fb() {
    if (start_mmap_fb) {
        if (munmap(start_mmap_fb, fbfs.smem_len) == -1) {
            perror("Error unmapping /dev/fb0 to memory in close_fb()");
        }
    }
    if (close(fd) == -1) {
        perror("Error closing /dev/fb0 in close_fb()");
        exit(1);
    }
}

void setPixel(int x, int y) {
    // FOR NOW ONLY ROW 0 - COL X
    unsigned long rOff = fbvs.red.offset/fbvs.red.length;
    unsigned long bOff = fbvs.blue.offset/fbvs.blue.length;
    unsigned long gOff = fbvs.green.offset/fbvs.green.length;
    *(screen_data + x + rOff) = 0xff;
    *(screen_data + x + bOff) = 0x00;
    *(screen_data + x + gOff) = 0x00;
}

void draw() {
    setPixel(0, 0);
}