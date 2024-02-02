#ifndef STACK_H_
#define STACK_H_

#include "types.h"

#define MAX_STACK 16

extern word_t stack[MAX_STACK];
extern byte_t stack_pointer;
extern word_t I;

#endif // STACK_H_