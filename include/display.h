#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "types.h"

#define MAX_DISP_ROW 64
#define MAX_DISP_COL 32
#define MAX_SPRITES 80

extern byte_t display[MAX_DISP_ROW * MAX_DISP_COL];
extern byte_t sprites[MAX_SPRITES];

#endif // DISPLAY_H_