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

static unsigned short int createAddress(unsigned char high, unsigned char low) {
    return (unsigned short int)(high << 8) + (unsigned short int)low;
}

typedef void (*instructionFunction)(unsigned char high, unsigned char low);

static void sys_addr_func (unsigned char high, unsigned char low) {

}

static void cls_func (unsigned char high, unsigned char low) {
    
    // debug
    // unsigned char high_debug = 0x12;
    // unsigned char low_debug = 0x34;
    // unsigned short int ca = createAddress(high_debug, low_debug);
    // printf(">>>>%d<<<<", ca);
}

static void ret_func (unsigned char high, unsigned char low) {

}

static void jp_addr_func (unsigned char high, unsigned char low) {

}

static void call_addr_func (unsigned char high, unsigned char low) {

}

static void se_vx_byte_func (unsigned char high, unsigned char low) {

}

static void sne_vx_btye_func (unsigned char high, unsigned char low) {

}

static void se_vx_vy_func (unsigned char high, unsigned char low) {

}

static void ld_vx_byte_func (unsigned char high, unsigned char low) {

}

static void add_vx_byte_func (unsigned char high, unsigned char low) {

}

static void ld_vx_vy_func (unsigned char high, unsigned char low) {

}

static void or_vx_vy_func (unsigned char high, unsigned char low) {

}

static void and_vx_vy_func (unsigned char high, unsigned char low) {

}

static void xor_vx_vy_func (unsigned char high, unsigned char low) {

}

static void add_vx_vy_func (unsigned char high, unsigned char low) {

}

static void sub_vx_vy_func (unsigned char high, unsigned char low) {

}

static void shr_vx_vy_func (unsigned char high, unsigned char low) {

}

static void subn_vx_vy_func (unsigned char high, unsigned char low) {

}

static void shl_vx_vy_func (unsigned char high, unsigned char low) {

}

static void sne_vx_vy_func (unsigned char high, unsigned char low) {

}

static void ld_i_addr_func (unsigned char high, unsigned char low) {

}

static void jp_v0_addr_func (unsigned char high, unsigned char low) {

}

static void rnd_vx_byte_func (unsigned char high, unsigned char low) {

}

static void drw_vx_vy_nibble_func (unsigned char high, unsigned char low) {

}

static void skp_vx_func (unsigned char high, unsigned char low) {

}

static void sknp_vx_func (unsigned char high, unsigned char low) {

}

static void ld_vx_dt_func (unsigned char high, unsigned char low) {

}

static void ld_vx_k_func (unsigned char high, unsigned char low) {

}

static void ld_dt_vx_func (unsigned char high, unsigned char low) {

}

static void ld_st_vx_func (unsigned char high, unsigned char low) {

}

static void add_i_vx_func (unsigned char high, unsigned char low) {

}

static void ld_f_vx_func (unsigned char high, unsigned char low) {

}

static void ld_b_vx_func (unsigned char high, unsigned char low) {

}

static void ld_i_vx_func (unsigned char high, unsigned char low) {

}

static void ld_vx_i_func (unsigned char high, unsigned char low) {

}

struct Instructions {
    instructionFunction instructionFunctions[35]; // 35 instructions
    void init() {
        instructionFunctions[SYS_ADDR] = sys_addr_func;
        instructionFunctions[CLS] = cls_func;
        instructionFunctions[RET] = ret_func;
        instructionFunctions[JP_ADDR] = jp_addr_func;
        instructionFunctions[CALL_ADDR] = call_addr_func;
        instructionFunctions[SE_VX_BYTE] = se_vx_byte_func;
        instructionFunctions[SNE_VX_BYTE] = sne_vx_btye_func;
        instructionFunctions[SE_VX_VY] = se_vx_vy_func;
        instructionFunctions[LD_VX_BYTE] = ld_vx_byte_func;
        instructionFunctions[ADD_VX_BYTE] = add_vx_byte_func;
        instructionFunctions[LD_VX_VY] = ld_vx_vy_func;
        instructionFunctions[OR_VX_VY] = or_vx_vy_func;
        instructionFunctions[AND_VX_VY] = and_vx_vy_func;
        instructionFunctions[XOR_VX_VY] = xor_vx_vy_func;
        instructionFunctions[ADD_VX_VY] = add_vx_vy_func;
        instructionFunctions[SUB_VX_VY] = sub_vx_vy_func;
        instructionFunctions[SHR_VX_VY] = shr_vx_vy_func;
        instructionFunctions[SUBN_VX_VY] = subn_vx_vy_func;
        instructionFunctions[SHL_VX_VY] = shl_vx_vy_func;
        instructionFunctions[SNE_VX_VY] = sne_vx_vy_func;
        instructionFunctions[LD_I_ADDR] = ld_i_addr_func;
        instructionFunctions[JP_V0_ADDR] = jp_v0_addr_func;
        instructionFunctions[RND_VX_BYTE] = rnd_vx_byte_func;
        instructionFunctions[DRW_VX_VY_NIBBLE] = drw_vx_vy_nibble_func;
        instructionFunctions[SKP_VX] = skp_vx_func;
        instructionFunctions[SKNP_VX] = sknp_vx_func;
        instructionFunctions[LD_VX_DT] = ld_vx_dt_func;
        instructionFunctions[LD_VX_K] = ld_vx_k_func;
        instructionFunctions[LD_DT_VX] = ld_dt_vx_func;
        instructionFunctions[LD_ST_VX] = ld_st_vx_func;
        instructionFunctions[ADD_I_VX] = add_i_vx_func;
        instructionFunctions[LD_F_VX] = ld_f_vx_func;
        instructionFunctions[LD_B_VX] = ld_b_vx_func;
        instructionFunctions[LD_I_VX] = ld_i_vx_func;
        instructionFunctions[LD_VX_I] = ld_vx_i_func;
    }
};
