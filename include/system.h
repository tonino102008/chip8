#ifndef CPU_H_
#define CPU_H_

#include "types.h"

extern byte_t delay_timer;
extern byte_t sound_timer;

void init_memory();
void opcode_switch(word_t opcode);

#endif // CPU_H_