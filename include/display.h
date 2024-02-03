#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "types.h"

#include "linux/fb.h"

#define MAX_DISP_ROW 64
#define MAX_DISP_COL 32
#define MAX_SPRITES 80

extern byte_t display[MAX_DISP_ROW * MAX_DISP_COL];
extern byte_t sprites[MAX_SPRITES];

extern int fb;
extern struct fb_fix_screeninfo fbfs;
extern struct fb_var_screeninfo fbvs;

void init_fb();
void close_fb();

#endif // DISPLAY_H_