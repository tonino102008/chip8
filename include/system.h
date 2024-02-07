#ifndef CPU_H_
#define CPU_H_

#include "types.h"

#define CPU_CLOCK 600.0 // Hertz
#define TIMER_CLOCK 60.0 // Hertz
#define TIMER2CPU (CPU_CLOCK / TIMER_CLOCK)
#define TICK_FREQ (1.0 / CPU_CLOCK) * 1000.0 // ms

extern byte_t delay_timer;
extern byte_t sound_timer;

void init_memory();
void opcode_switch(word_t opcode);

#endif // CPU_H_