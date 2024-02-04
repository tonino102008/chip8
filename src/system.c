#include "../include/register.h"
#include "../include/memory.h"
#include "../include/stack.h"
#include "../include/display.h"
#include "../include/system.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

byte_t delay_timer = 0x00;
byte_t sound_timer = 0x00;

void init_memory() {
    memcpy(memory, sprites, (size_t)MAX_SPRITES);
}

void opcode_switch(word_t opcode) {
    switch (opcode & 0xF000) {
        case 0x0000: // OPCODE 0NNN NOT NECESSARY -> IGNORING IT
            switch (opcode & 0x00F0) {
                case 0x00E0:
                    switch (opcode & 0x000F) {
                        case 0x0000:
                            printf("OpCode: 0x%04x\n", opcode);
                            clean_screen();
                            printf("Screen cleared.\n");
                        break;
                        case 0x000E:
                            printf("OpCode: 0x%04x - PC: %04x - SP: %02x\n", opcode, PC, stack_pointer);
                            PC = stack[stack_pointer];
                            stack_pointer -= 0x01;
                            printf("Result: PC = %04x - SP = %02x\n", PC, stack_pointer);
                        break;
                        default:
                            printf("Unsupported OpCode: 0x%04x\n", opcode);
                            exit(0);
                    }
                break;
                default:
                    printf("Unsupported OpCode: 0x%04x\n", opcode);
                    exit(0);
            }
            break;
        case 0x1000:
            printf("OpCode: 0x%04x - PC: %04x\n", opcode, PC);
            PC = opcode & 0x0FFF;
            printf("Result: PC = %04x\n", PC);
            break;
        case 0x2000:
            printf("OpCode: 0x%04x - PC: %04x - SP: %02x\n", opcode, PC, stack_pointer);
            stack_pointer += 0x01;
            stack[stack_pointer] = PC;
            PC = opcode & 0x0FFF;
            printf("Result: PC = %04x - SP = %02x\n", PC, stack_pointer);
            break;
        case 0x3000:
            printf("OpCode: 0x%04x - X = %02x - PC: %04x\n", opcode, (opcode & 0x0F00) >> 8, PC);
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) PC += 0x0002;
            printf("Result: X = %02x - PC = %04x\n", V[(opcode & 0x0F00) >> 8], PC);
            break;
        case 0x4000:
            printf("OpCode: 0x%04x - X = %02x - PC: %04x\n", opcode, (opcode & 0x0F00) >> 8, PC);
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) PC += 0x0002;
            printf("Result: X = %02x - PC = %04x\n", V[(opcode & 0x0F00) >> 8], PC);
            break;
        case 0x5000:
            printf("OpCode: 0x%04x - X = %02x - Y = %02x - PC: %04x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, PC);
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) PC += 0x0002;
            printf("Result: X = %02x - Y = %02x - PC = %04x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], PC);
            break;
        case 0x6000:
            printf("OpCode: 0x%04x - X = %02x\n", opcode, (opcode & 0x0F00) >> 8);
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            printf("Result: X = %02x\n", V[(opcode & 0x0F00) >> 8]);
            break;
        case 0x7000:
            printf("OpCode: 0x%04x - X = %02x\n", opcode, (opcode & 0x0F00) >> 8);
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            printf("Result: X = %02x\n", V[(opcode & 0x0F00) >> 8]);
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
            case 0x0000:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);
                break;
            case 0x0001:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);
                break;
            case 0x0002:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);
                break;
            case 0x0003:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);
                break;
            case 0x0004: // TO BE CHECKED
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                if ((word_t)V[(opcode & 0x0F00) >> 8] + (word_t)V[(opcode & 0x00F0) >> 4] > 0x00FF) 
                    V[0x0F] = 0x01;
                else 
                    V[0x0F] = 0x00;
                V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x - F = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[0x0F]);
                break;
            case 0x0005:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                    V[0x0F] = 0x01;
                else 
                    V[0x0F] = 0x00;
                V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                printf("Result: X = %02x - Y = %02x - F = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[0x0F]);
                break;
            case 0x0006: // TO BE CHECKED
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                if ((V[(opcode & 0x0F00) >> 8] & 0x0F) == 0x01)
                    V[0x0F] = 0x01;
                else 
                    V[0x0F] = 0x00;
                V[(opcode & 0x0F00) >> 8] /= 2;
                printf("Result: X = %02x - Y = %02x - F = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[0x0F]);
                break;
            case 0x0007:
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                    V[0x0F] = 0x01;
                else 
                    V[0x0F] = 0x00;
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                printf("Result: X = %02x - Y = %02x - F = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[0x0F]);
                break;
            case 0x000E: // TO BE CHECKED
                printf("OpCode: 0x%04x - X = %02x - Y = %02x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                if ((V[(opcode & 0x0F00) >> 8] & 0xF0) == 0x10)
                    V[0x0F] = 0x01;
                else 
                    V[0x0F] = 0x00;
                V[(opcode & 0x0F00) >> 8] *= 2;
                printf("Result: X = %02x - Y = %02x - F = %02x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[0x0F]);
                break;
            default:
                printf("Unsupported OpCode: 0x%04x\n", opcode);
                exit(0);
            }
            break;
        case 0x9000:
            switch(opcode & 0x000F) {
                case 0x0000:
                    printf("OpCode: 0x%04x - X = %02x - Y = %02x - PC: %04x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, PC);
                    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) PC += 0x0002;
                    printf("Result: X = %02x - Y = %02x - PC = %04x\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], PC);
                    break;
                default:
                    printf("Unsupported OpCode: 0x%04x\n", opcode);
                    exit(0);
            }
            break;
        case 0xA000:
            printf("OpCode: 0x%04x - I = %04x\n", opcode, I);
            I = opcode & 0x0FFF;
            printf("Result: I = %04x\n", I);
            break;
        case 0xB000:
            printf("OpCode: 0x%04x - PC = %04x\n", opcode, PC);
            PC = (opcode & 0x0FFF) + (word_t)V[0x00];
            printf("Result: 0 = %02x - PC = %04x\n", V[0x00], PC);
            break;
        case 0xC000:
            printf("OpCode: 0x%04x - X = %04x\n", opcode, (opcode & 0x0F00) >> 8);
            V[(opcode & 0x0F00) >> 8] = (byte_t)rand() & (byte_t)(opcode & 0x00FF);
            printf("Result: X = %04x\n", V[(opcode & 0x0F00) >> 8]);
            break;
        case 0xD000:
            printf("OpCode: 0x%04x - X = %04x - Y = %04x\n", opcode, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
            //sprite = memory[I + (word_t)i];
            for (byte_t i = V[(opcode & 0x0F00) >> 8]; i < V[(opcode & 0x0F00) >> 8] + (opcode & 0x000F); i++) {
                for (byte_t j = V[(opcode & 0x00F0) >> 4]; j < V[(opcode & 0x00F0) >> 4] + 0x08; j++) {
                    setPixel(i, j, WHITE);
                }
            }
            printf("Screen drawn.\n");
            break;
        case 0xF000:
            switch (opcode & 0x00F0) {
                case 0x0000:
                    switch (opcode & 0x000F) {
                        case 0x0007:
                            printf("OpCode: 0x%04x - X = %02x - DT = %02x\n", opcode, (opcode & 0x0F00) >> 8, delay_timer);
                            V[(opcode & 0x0F00) >> 8] = delay_timer;
                            printf("Result: X = %02x - DT = %02x\n", V[(opcode & 0x0F00) >> 8], delay_timer);
                            break;
                        default:
                            printf("Unsupported OpCode: 0x%04x\n", opcode);
                            exit(0);
                    }
                    break;
                case 0x0010:
                    switch (opcode & 0x000F) {
                        case 0x0005:
                            printf("OpCode: 0x%04x - X = %02x - DT = %02x\n", opcode, (opcode & 0x0F00) >> 8, delay_timer);
                            delay_timer = V[(opcode & 0x0F00) >> 8];
                            printf("Result: X = %02x - DT = %02x\n", V[(opcode & 0x0F00) >> 8], delay_timer);
                            break;
                        case 0x0008:
                            printf("OpCode: 0x%04x - X = %02x - ST = %02x\n", opcode, (opcode & 0x0F00) >> 8, sound_timer);
                            sound_timer = V[(opcode & 0x0F00) >> 8];
                            printf("Result: X = %02x - ST = %02x\n", V[(opcode & 0x0F00) >> 8], sound_timer);
                            break;
                        case 0x000E:
                            printf("OpCode: 0x%04x - X = %02x - I = %04x\n", opcode, (opcode & 0x0F00) >> 8, I);
                            I += (word_t)V[(opcode & 0x0F00) >> 8];
                            printf("Result: X = %02x - I = %04x\n", V[(opcode & 0x0F00) >> 8], I);
                            break;
                        default:
                            printf("Unsupported OpCode: 0x%04x\n", opcode);
                            exit(0);
                    }
                    break;
                case 0x0050:
                    switch (opcode & 0x000F) {
                        case 0x0005:
                            for (byte_t i = 0x00; i <= (byte_t)((opcode & 0x0F00) >> 8); i += 0x01) {
                                printf("OpCode: 0x%04x - X = %02x - I = %04x\n", opcode, i, I);
                                memory[I] = V[i];
                                I += (word_t)0x01;
                                printf("Result: X = %02x - M = %02x - I = %04x\n", V[i], memory[I - (word_t)0x01], I);
                            }
                            break;
                        default:
                            printf("Unsupported OpCode: 0x%04x\n", opcode);
                            exit(0);
                    }
                    break;
                case 0x0060:
                    switch (opcode & 0x000F) {
                        case 0x0005:
                            for (byte_t i = 0x00; i <= (byte_t)((opcode & 0x0F00) >> 8); i += 0x01) {
                                printf("OpCode: 0x%04x - X = %02x - I = %04x\n", opcode, i, I);
                                V[i] = memory[I];
                                I += (word_t)0x01;
                                printf("Result: X = %02x - M = %02x - I = %04x\n", V[i], memory[I - (word_t)0x01], I);
                            }
                            break;
                        default:
                            printf("Unsupported OpCode: 0x%04x\n", opcode);
                            exit(0);
                    }
                    break;
                default:
                    printf("Unsupported OpCode: 0x%04x\n", opcode);
                    exit(0);
            }
            break;
        default:
            printf("Unsupported OpCode: 0x%04x\n", opcode);
            exit(0);
    }
}