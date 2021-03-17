#include "headers/cpu.h"

/* instructions
    0nnn - SYS addr
    00E0 - CLS
    00EE - RET
    1nnn - JP addr
    2nnn - CALL addr
    3xkk - SE Vx, byte
    4xkk - SNE Vx, byte
    5xy0 - SE Vx, Vy
    6xkk - LD Vx, byte
    7xkk - ADD Vx, byte
    8xy0 - LD Vx, Vy
    8xy1 - OR Vx, Vy
    8xy2 - AND Vx, Vy
    8xy3 - XOR Vx, Vy
    8xy4 - ADD Vx, Vy
    8xy5 - SUB Vx, Vy
    8xy6 - SHR Vx {, Vy}
    8xy7 - SUBN Vx, Vy
    8xyE - SHL Vx {, Vy}
    9xy0 - SNE Vx, Vy
    Annn - LD I, addr
    Bnnn - JP V0, addr
    Cxkk - RND Vx, byte
    Dxyn - DRW Vx, Vy, nibble
    Ex9E - SKP Vx
    ExA1 - SKNP Vx
    Fx07 - LD Vx, DT
    Fx0A - LD Vx, K
    Fx15 - LD DT, Vx
    Fx18 - LD ST, Vx
    Fx1E - ADD I, Vx
    Fx29 - LD F, Vx
    Fx33 - LD B, Vx
    Fx55 - LD [I], Vx
    Fx65 - LD Vx, [I]
*/

void Cpu::step() {
    // takes next "clock cycle" step
    
    // get next instruction from PC
    // unsigned short int nextInstruction = getNextInstruction()

    // determine how to process instruction
    // processInstruction(nextInstruciton)


}

void Cpu::decrementSoundAndTime() {
    // check if DT or ST are "active" and decrement them
    if (this->DT > 0) this->DT--;
    if (this->ST > 0) this->ST--;
}

Instruction Cpu::getInstruction(unsigned char high, unsigned char low) {
    unsigned char highNibble = getHighNibble(high);
    if (highNibble == 0x00) { // determine which 0 instruction is used
        if (low == 0xe0) return CLS;
        else if (low == 0xee) return RET;
        else return SYS_ADDR;
    } else if (highNibble == 0x01) { // determine which 1 instruction is used
        return JP_ADDR;
    } else if (highNibble == 0x02) { // determine which 2 instruction is used
        return CALL_ADDR;
    } else if (highNibble == 0x03) { // determine which 3 instruction is used
        return SE_VX_BYTE;
    } else if (highNibble == 0x04) { // determine which 4 instruction is used
        return SNE_VX_BYTE;
    } else if (highNibble == 0x05) { // determine which 5 instruction is used
        return SE_VX_VY;
    } else if (highNibble == 0x06) { // determine which 6 instruction is used
        return LD_VX_BYTE;
    } else if (highNibble == 0x07) { // determine which 7 instruction is used
        return ADD_VX_BYTE;
    } else if (highNibble == 0x08) { // determine which 8 instruction is used
        unsigned char lowernibble = getLowNibble(low);
        if (lowernibble == 0x00) return LD_VX_VY;
        else if (lowernibble == 0x01) return OR_VX_VY;
        else if (lowernibble == 0x02) return AND_VX_VY;
        else if (lowernibble == 0x03) return XOR_VX_VY;
        else if (lowernibble == 0x04) return ADD_VX_VY;
        else if (lowernibble == 0x05) return SUB_VX_VY;
        else if (lowernibble == 0x06) return SHR_VX_VY;
        else if (lowernibble == 0x07) return SUBN_VX_VY;
        else if (lowernibble == 0x0e) return SHL_VX_VY;
    } else if (highNibble == 0x09) { // determine which 9 instruction is used
        return SNE_VX_VY;
    } else if (highNibble == 0x0a) { // determine which a instruction is used
        return LD_I_ADDR;
    } else if (highNibble == 0x0b) { // determine which b instruction is used
        return JP_V0_ADDR;
    } else if (highNibble == 0x0c) { // determine which c instruction is used
        return RND_VX_BYTE;
    } else if (highNibble == 0x0d) { // determine which d instruction is used
        return DRW_VX_VY_NIBBLE;
    } else if (highNibble == 0x0e) { // determine which e instruction is used
        if (low == 0x9e) return SKP_VX;
        else if (low == 0xa1) return SKNP_VX;
    } else if (highNibble == 0x0f) { // determine which f instruction is used
        if (low == 0x07) return LD_VX_DT;
        else if (low == 0x0a) return LD_VX_K;
        else if (low == 0x15) return LD_DT_VX;
        else if (low == 0x18) return LD_ST_VX;
        else if (low == 0x1e) return ADD_I_VX;
        else if (low == 0x29) return LD_F_VX;
        else if (low == 0x33) return LD_B_VX;
        else if (low == 0x55) return LD_I_VX;
        else if (low == 0x65) return LD_VX_I;
    }
    // an illegal opcode has been given
    return ERROR;
}

unsigned char Cpu::getHighNibble(unsigned char byte) {
    return (byte & 0b11110000) >> 4;
}

unsigned char Cpu::getLowNibble(unsigned char byte) {
    return byte & 0b00001111;
}