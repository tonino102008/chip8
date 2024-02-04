#include "../include/display.h"
#include "../include/register.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys/ioctl.h"
#include "sys/mman.h"

byte_t display[MAX_DISP_COL * MAX_DISP_ROW] = {(byte_t)BLACK};
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
char *start_mmap_fb;
Screen screen;

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
    start_mmap_fb = (char*)mmap(NULL, fbfs.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
    if (start_mmap_fb == (void*)-1) {
        perror("Error mapping /dev/fb0 to memory in init_fb()");
        close_fb();
        exit(1);
    }
    unsigned long off = fbfs.smem_start % (unsigned long)sysconf(_SC_PAGESIZE);
    screen = (Screen){  .phys_res_x = MAX_DISP_COL,
                        .phys_res_y = MAX_DISP_ROW,
                        .virt_res_x = MAX_VDISP_COL,
                        .virt_res_y = MAX_VDISP_ROW,
                        .virt_off_x = FB_OFF_COL,
                        .virt_off_y = FB_OFF_ROW,
                        .r_off = fbvs.red.offset/fbvs.red.length,
                        .g_off = fbvs.green.offset/fbvs.green.length,
                        .b_off = fbvs.blue.offset/fbvs.blue.length,
                        .data = start_mmap_fb + off};
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

void setPixel(int x, int y, int black) {
    int off = y * fbfs.line_length + x * (fbvs.bits_per_pixel >> 3);
    *(screen.data + off + screen.r_off) = 0xff * black;
    *(screen.data + off + screen.g_off) = 0xff * black;
    *(screen.data + off + screen.b_off) = 0xff * black;
}

void setVPixel(int offset_x, int offset_y, int x, int y, int black) {
    if ((display[x + y * screen.phys_res_x] ^ (byte_t)black) != display[x + y * screen.phys_res_x]) V[0x0F] = 0x01;
    display[x + y * screen.phys_res_x] = display[x + y * screen.phys_res_x] ^ (byte_t)black;
    int xV = offset_x + x * VTPR_RES;
    int yV = offset_y + y * VTPR_RES;
    for (int i = 0; i < VTPR_RES; i++) {
        for (int j = 0; j < VTPR_RES; j++) {
            setPixel(xV + i, yV + j, display[x + y * screen.phys_res_x]);
        }
    }
}

void draw_screen() {
    for (int i = screen.virt_off_x; i < screen.virt_off_x + screen.virt_res_x; i++) {
        for (int j = screen.virt_off_y; j < screen.virt_off_y + screen.virt_res_y; j++) {
            setPixel(i, j, WHITE);
        }
    }
}

void draw_Vscreen() {
    for (int i = 0; i < screen.phys_res_x; i++) {
        for (int j = 0; j < screen.phys_res_y; j++) {
            setVPixel(screen.virt_off_x, screen.virt_off_y, i, j, WHITE);
        }
    }
}

void clean_screen() {
    for (int i = screen.virt_off_x; i < screen.virt_off_x + screen.virt_res_x; i++) {
        for (int j = screen.virt_off_y; j < screen.virt_off_y + screen.virt_res_y; j++) {
            setPixel(i, j, BLACK);
        }
    }
}

void clean_Vscreen() {
    memset(display, BLACK, MAX_DISP_COL * MAX_DISP_ROW);
    for (int i = 0; i < screen.phys_res_x; i++) {
        for (int j = 0; j < screen.phys_res_y; j++) {
            setVPixel(screen.virt_off_x, screen.virt_off_y, i, j, BLACK);
        }
    }
}