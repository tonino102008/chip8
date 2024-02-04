#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "types.h"

#include "linux/fb.h"

#define MAX_DISP_COL 64
#define MAX_DISP_ROW 32
#define MAX_SPRITES 80
#define MAX_VDISP_COL MAX_DISP_COL * 5
#define MAX_VDISP_ROW MAX_DISP_ROW * 5
#define FB_OFF_COL (1024 - MAX_VDISP_COL) / 2
#define FB_OFF_ROW (768 - MAX_VDISP_ROW) / 2
#define BLACK 0x00
#define WHITE 0x01

extern byte_t display[MAX_DISP_ROW * MAX_DISP_COL];
extern byte_t sprites[MAX_SPRITES];

typedef struct {
    int phys_res_x;
    int phys_res_y;
    int virt_res_x;
    int virt_res_y;
    int virt_off_x;
    int virt_off_y;
    int r_off;
    int g_off;
    int b_off;
    char* data;
} Screen;

extern int fd;
extern char *start_mmap_fb;
extern Screen screen;
extern struct fb_fix_screeninfo fbfs;
extern struct fb_var_screeninfo fbvs;

void init_fb();
void close_fb();
void setPixel(int, int, int); // x, y refers to x, y screen resolution coordinates
void draw_screen();
void clean_screen();

#endif // DISPLAY_H_