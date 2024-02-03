#include "../googletest/googletest/include/gtest/gtest.h"

extern "C" {
    #include "../../include/types.h"
    #include "../../include/system.h"
    #include "../../include/memory.h"
    #include "../../include/stack.h"
    #include "../../include/register.h"
    #include "../../include/display.h"
}

namespace {

TEST(DummyTest, Test) {
    EXPECT_EQ(1, 1);
}

TEST(Memory, Init) {
    EXPECT_EQ((size_t)MAX_MEM, sizeof(memory)/sizeof(byte_t));
    for (int i = 0; i < MAX_MEM; i++) EXPECT_EQ((byte_t)0, memory[i]);
    EXPECT_EQ((word_t)0x0200, PC);
}

TEST(Stack, Init) {
    EXPECT_EQ((size_t)MAX_STACK, sizeof(stack)/sizeof(word_t));
    for (int i = 0; i < MAX_STACK; i++) EXPECT_EQ((word_t)0x0000, stack[i]);
    EXPECT_EQ((byte_t)0x00, stack_pointer);
    EXPECT_EQ((word_t)0x0000, I);
}

TEST(System, OpCode0x00E0) {
    memory[PC] = 0x00;
    memory[PC + 1] = 0xE0;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x00EE) {
    stack_pointer = 0x01;
    memory[PC] = 0x00;
    memory[PC + 1] = 0xEE;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x01NNN) {
    memory[PC] = 0x14;
    memory[PC + 1] = 0x56;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x02NNN) {
    memory[PC] = 0x22;
    memory[PC + 1] = 0x34;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x3XKK) {
    V[0x01] = 0x01;
    memory[PC] = 0x31;
    memory[PC + 1] = 0x02;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    opcode_switch(opcode);
}

TEST(System, OpCode0x4XKK) {
    V[0x01] = 0x01;
    memory[PC] = 0x41;
    memory[PC + 1] = 0x02;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    opcode_switch(opcode);
}

TEST(System, OpCode0x5XY0) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x51;
    memory[PC + 1] = 0x20;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    opcode_switch(opcode);
}

TEST(System, OpCode0x6XKK) {
    V[0x01] = 0x01;
    memory[PC] = 0x61;
    memory[PC + 1] = 0x02;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x7XKK) {
    V[0x01] = 0x01;
    memory[PC] = 0x71;
    memory[PC + 1] = 0x02;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY0) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x20;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY1) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x21;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY2) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x22;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY3) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x23;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY4) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x24;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0xFF;
    V[0x02] = 0x01;
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY5) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x25;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    V[0x02] = 0x01;
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XY6) {
    V[0x01] = 0x01;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x26;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    opcode_switch(opcode);

}

TEST(System, OpCode0x08XY7) {
    V[0x01] = 0x01;
    V[0x02] = 0x02;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x27;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x02;
    V[0x02] = 0x01;
    opcode_switch(opcode);
}

TEST(System, OpCode0x08XYE) {
    V[0x01] = 0x00;
    memory[PC] = 0x81;
    memory[PC + 1] = 0x2E;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x01;
    opcode_switch(opcode);

    V[0x01] = 0x80;
    opcode_switch(opcode);

    V[0x01] = 0xFF;
    opcode_switch(opcode);
}

TEST(System, OpCode0x09XY0) {
    V[0x01] = 0x02;
    V[0x02] = 0x02;
    memory[PC] = 0x91;
    memory[PC + 1] = 0x20;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);

    V[0x01] = 0x01;
    opcode_switch(opcode);
}

TEST(System, OpCode0xANNN) {
    I = 0x0000;
    memory[PC] = 0xA1;
    memory[PC + 1] = 0x23;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xBNNN) {
    V[0x00] = 0x01;
    memory[PC] = 0xB3;
    memory[PC + 1] = 0x45;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xCXNN) {
    V[0x01] = 0x01;
    memory[PC] = 0xC1;
    memory[PC + 1] = 0x48;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX07) {
    V[0x01] = 0x01;
    memory[PC] = 0xF1;
    memory[PC + 1] = 0x07;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX15) {
    V[0x01] = 0x01;
    memory[PC] = 0xF1;
    memory[PC + 1] = 0x15;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX18) {
    V[0x01] = 0x01;
    memory[PC] = 0xF1;
    memory[PC + 1] = 0x18;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX1E) {
    V[0x01] = 0x01;
    memory[PC] = 0xF1;
    memory[PC + 1] = 0x1E;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX55) {
    for (byte_t i = 0x00; i <= 0x0F; i += 0x01) V[i] = i;
    memory[PC] = 0xFF;
    memory[PC + 1] = 0x55;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(System, OpCode0xFX65) {
    I -= 0x10;
    memory[PC] = 0xFF;
    memory[PC + 1] = 0x65;
    word_t opcode = (memory[PC] << 8) | memory[PC+1];
    opcode_switch(opcode);
}

TEST(Display, InitMemory) {
    init_memory();
    for (int i = 0; i < MAX_SPRITES; i++) EXPECT_EQ(sprites[i], memory[i]);
}

TEST(Display, InitCloseFb) {
    init_fb();
    close_fb();
}

TEST(Display, DrawFb) {
    init_fb();
    draw();
    close_fb();
}

}