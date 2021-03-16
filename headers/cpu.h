#pragma once

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
        char ram[4096]; // 4k ram
        char registers[16]; // 16 8-bit registers
        short int I; // 16-bit address regsiter 
        char time; // 8-bit time register
        char sound;  // 8-bit sound register
        short int PC; // 16-bit program counter
        char SP; // 8-bit stack pointer
        short int stack[16]; // 16 slot 16-bit stack 
    public:
        Cpu();
        ~Cpu();
};