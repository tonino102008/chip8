#include "../include/display.h"
#include "linux/fb.h"

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