#pragma once
#include <iostream>

enum Instruction {
    ERROR = -1,
    SYS_ADDR = 0,
    CLS,
    RET,
    JP_ADDR,
    CALL_ADDR,
    SE_VX_BYTE,
    SNE_VX_BYTE,
    SE_VX_VY,
    LD_VX_BYTE,
    ADD_VX_BYTE,
    LD_VX_VY,
    OR_VX_VY,
    AND_VX_VY,
    XOR_VX_VY,
    ADD_VX_VY,
    SUB_VX_VY,
    SHR_VX_VY,
    SUBN_VX_VY,
    SHL_VX_VY,
    SNE_VX_VY,
    LD_I_ADDR,
    JP_V0_ADDR,
    RND_VX_BYTE,
    DRW_VX_VY_NIBBLE,
    SKP_VX,
    SKNP_VX,
    LD_VX_DT,
    LD_VX_K,
    LD_DT_VX,
    LD_ST_VX,
    ADD_I_VX,
    LD_F_VX,
    LD_B_VX,
    LD_I_VX,
    LD_VX_I
};

enum registers {
    V0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    Va,
    Vb,
    Vc,
    Vd,
    Ve,
    Vf // flag for certain instructions, to not be used by programs
};

class Cpu {
    private:
        unsigned char ram[4096]; // 4k ram
        unsigned char registers[16]; // 16 8-bit registers
        unsigned short int I; // 16-bit address regsiter 
        unsigned char DT; // 8-bit time register
        unsigned char ST;  // 8-bit sound register
        unsigned short int PC; // 16-bit program counter
        unsigned char SP; // 8-bit stack pointer
        unsigned short int stack[16]; // 16 slot 16-bit stack 
    public:
        Cpu() {
            // init characters in "interpreter" portion of ram
            // this takes up 16 * 5 (80) bytes at the start of ram
            unsigned char characters[16][5] = {
                {0xf0, 0x90, 0x90, 0x90, 0xf0}, // character 0
                {0x20, 0x60, 0x20, 0x20, 0x70}, // character 1
                {0xf0, 0x10, 0xf0, 0x80, 0xf0}, // character 2
                {0xf0, 0x10, 0xf0, 0x10, 0xf0}, // character 3
                {0x90, 0x90, 0xF0, 0x10, 0x10}, // character 4
                {0xf0, 0x80, 0xf0, 0x10, 0xf0}, // character 5
                {0xf0, 0x80, 0xf0, 0x90, 0xf0}, // character 6
                {0xf0, 0x10, 0x20, 0x40, 0x40}, // character 7
                {0xf0, 0x90, 0xf0, 0x90, 0xf0}, // character 8
                {0xf0, 0x90, 0xf0, 0x10, 0xf0}, // character 9
                {0xf0, 0x90, 0xf0, 0x90, 0x90}, // character a
                {0xe0, 0x90, 0xe0, 0x90, 0xe0}, // character b
                {0xf0, 0x80, 0x80, 0x80, 0xf0}, // character c
                {0xe0, 0x90, 0x90, 0x90, 0xe0}, // character d
                {0xf0, 0x80, 0xf0, 0x80, 0xf0}, // character e
                {0xf0, 0x80, 0xf0, 0x80, 0x80} // character f
            };

            for (int i = 0; i < 16; i++) { // 16 characters
                for (int j = 0; j < 5; j++) { // 5 bytes per character
                    ram[5*i + j] = characters[i][j];
                }
            }

            // testing low high nibble getters
            // unsigned char test = 0b10111101;
            // printf("high nibble %d\n", getHighNibble(test));
            // printf("low nibble %d\n", getLowNibble(test));

            // display characters for debug
            // for (int i = 0; i < 16; i++) { // 16 characters
            //     for (int j = 0; j < 5; j++) { // 5 bytes per character
            //         printf("%d\n", ram[5*i + j]);
            //     }
            // }
        };
        ~Cpu(){};
        void step();
        Instruction getInstruction(unsigned char high, unsigned char low);
        void decrementSoundAndTime();
        unsigned char getHighNibble(unsigned char byte);
        unsigned char getLowNibble(unsigned char byte);
};