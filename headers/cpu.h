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
    V0 = 0,
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
        std::vector<char>* pixels; // 64 x 32 pixel bitmap
        char keysPressed[16];
        char previousKeyPressed;
        char lastKeyPressed;
    public:
        Cpu(std::vector<char>* pixels, const char* filename);
        ~Cpu();
        void step();
        void decrementSoundAndTime();
        void loadGameRomIntoRam(const char* filename);
        char getPixelAtCoord(unsigned char x, unsigned char y);
        void setPixelAtCoord(unsigned char x, unsigned char y, char value);
        void updateKeysPressed(char keys[16]);
        void updateLastKeyPressed(char key);
        unsigned char getST();

        // instruction extraction and execution
        Instruction getInstruction(unsigned char high, unsigned char low);
        void executeInstruction(Instruction instruction, unsigned char high, unsigned char low);
        
        // bit manipulation methods
        unsigned char getHighByte(unsigned char word);
        unsigned char getLowByte(unsigned char word);
        unsigned char getHighNibble(unsigned char byte);
        unsigned char getLowNibble(unsigned char byte);
        unsigned short int createAddress(unsigned char high, unsigned char low);
        
        // instruction implementations
        void sys_addr_func(unsigned char high, unsigned char low);
        void cls_func(unsigned char high, unsigned char low);
        void ret_func(unsigned char high, unsigned char low);
        void jp_addr_func(unsigned char high, unsigned char low);
        void call_addr_func(unsigned char high, unsigned char low);
        void se_vx_byte_func(unsigned char high, unsigned char low);
        void sne_vx_btye_func(unsigned char high, unsigned char low);
        void se_vx_vy_func(unsigned char high, unsigned char low);
        void ld_vx_byte_func(unsigned char high, unsigned char low);
        void add_vx_byte_func(unsigned char high, unsigned char low);
        void ld_vx_vy_func(unsigned char high, unsigned char low);
        void or_vx_vy_func(unsigned char high, unsigned char low);
        void and_vx_vy_func(unsigned char high, unsigned char low);
        void xor_vx_vy_func(unsigned char high, unsigned char low);
        void add_vx_vy_func(unsigned char high, unsigned char low);
        void sub_vx_vy_func(unsigned char high, unsigned char low);
        void shr_vx_vy_func(unsigned char high, unsigned char low);
        void subn_vx_vy_func(unsigned char high, unsigned char low);
        void shl_vx_vy_func(unsigned char high, unsigned char low);
        void sne_vx_vy_func(unsigned char high, unsigned char low);
        void ld_i_addr_func(unsigned char high, unsigned char low);
        void jp_v0_addr_func(unsigned char high, unsigned char low);
        void rnd_vx_byte_func(unsigned char high, unsigned char low);
        void drw_vx_vy_nibble_func(unsigned char high, unsigned char low);
        void skp_vx_func(unsigned char high, unsigned char low);
        void sknp_vx_func(unsigned char high, unsigned char low);
        void ld_vx_dt_func(unsigned char high, unsigned char low);
        void ld_vx_k_func(unsigned char high, unsigned char low);
        void ld_dt_vx_func(unsigned char high, unsigned char low);
        void ld_st_vx_func(unsigned char high, unsigned char low);
        void add_i_vx_func(unsigned char high, unsigned char low);
        void ld_f_vx_func(unsigned char high, unsigned char low);
        void ld_b_vx_func(unsigned char high, unsigned char low);
        void ld_i_vx_func(unsigned char high, unsigned char low);
        void ld_vx_i_func(unsigned char high, unsigned char low);
};