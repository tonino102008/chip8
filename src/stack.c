#include "../include/stack.h"

word_t stack[MAX_STACK] = {(word_t)0x0000};
byte_t stack_pointer = (byte_t)0x00; // STACK GROWS DOWNWARD
word_t I = (word_t)0x0000;