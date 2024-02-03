#include "../include/display.h"

#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys/ioctl.h"

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
int fb;

void init_fb() {
    fb = open("/dev/fb0", O_RDWR);
    if (fb == -1) {
        perror("Error opening /dev/fb0 in init_fb()");
        exit(0);
    }
    if (ioctl(fb, FBIOGET_VSCREENINFO, &fbvs) == -1) {
        perror("Error getting VSCREENINFO from /dev/fb0 in init_fb()");
        exit(0);
    }
    if (ioctl(fb, FBIOGET_FSCREENINFO, &fbfs) == -1) {
        perror("Error getting FSCREENINFO from /dev/fb0 in init_fb()");
        exit(0);
    }
    printf("Screen dimensions: %d x %d\n", fbvs.width, fbvs.height);
}

void close_fb() {
    if (close(fb) == -1) {
        perror("Error closing /dev/fb0 in close_fb()");
        exit(0);
    }
}