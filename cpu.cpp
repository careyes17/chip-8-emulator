#include "headers/cpu.h"
#include <vector>

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

Cpu::Cpu(std::vector<char>* pixels) {
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

    this->pixels = pixels;

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

Cpu::~Cpu(){};

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

void Cpu::executeInstruction(Instruction instruction, unsigned char high, unsigned char low) {
    // switch statement targeting the correct function per instruction
    switch(instruction) {
        case ERROR:
            break;
        case SYS_ADDR:
            sys_addr_func(high, low);
            break;
        case CLS:
            cls_func(high, low);
            break;
        case RET:
            ret_func(high, low);
            break;
        case JP_ADDR: 
            jp_addr_func(high, low);
            break;
        case CALL_ADDR:
            call_addr_func(high, low);
            break;
        case SE_VX_BYTE:
            se_vx_byte_func(high, low);
            break;
        case SNE_VX_BYTE:
            sne_vx_btye_func(high, low);
            break;
        case SE_VX_VY:
            se_vx_vy_func(high, low);
            break;
        case LD_VX_BYTE:
            ld_vx_byte_func(high, low);
            break;
        case ADD_VX_BYTE:
            add_vx_byte_func(high, low);
            break;
        case LD_VX_VY:
            ld_vx_vy_func(high, low);
            break;
        case OR_VX_VY:
            or_vx_vy_func(high, low);
            break;
        case AND_VX_VY:
            and_vx_vy_func(high, low);
            break;
        case XOR_VX_VY:
            xor_vx_vy_func(high, low);
            break;
        case ADD_VX_VY:
            add_vx_vy_func(high, low);
            break;
        case SUB_VX_VY:
            sub_vx_vy_func(high, low);
            break;
        case SHR_VX_VY:
            shr_vx_vy_func(high, low);
            break;
        case SUBN_VX_VY:
            subn_vx_vy_func(high, low);
            break;
        case SHL_VX_VY:
            shl_vx_vy_func(high, low);
            break;
        case SNE_VX_VY:
            sne_vx_vy_func(high, low);
            break;
        case LD_I_ADDR:
            ld_i_addr_func(high, low);
            break;
        case JP_V0_ADDR:
            jp_v0_addr_func(high, low);
            break;
        case RND_VX_BYTE:
            rnd_vx_byte_func(high, low);
            break;
        case DRW_VX_VY_NIBBLE:
            drw_vx_vy_nibble_func(high, low);
            break;
        case SKP_VX:
            skp_vx_func(high, low);
            break;
        case SKNP_VX:
            sknp_vx_func(high, low);
            break;
        case LD_VX_DT:
            ld_vx_dt_func(high, low);
            break;
        case LD_VX_K:
            ld_vx_k_func(high, low);
            break;
        case LD_DT_VX:
            ld_dt_vx_func(high, low);
            break;
        case LD_ST_VX:
            ld_st_vx_func(high, low);
            break;
        case ADD_I_VX:
            add_i_vx_func(high, low);
            break;
        case LD_F_VX:
            ld_f_vx_func(high, low);
            break;
        case LD_B_VX:
            ld_b_vx_func(high, low);
            break;
        case LD_I_VX:
            ld_i_vx_func(high, low);
            break;
        case LD_VX_I:
            ld_vx_i_func(high, low);
            break;
        default:
            break;
    }
}

unsigned char Cpu::getHighByte(unsigned short int word) {
    return word >> 8;
}

unsigned char Cpu::getLowByte(unsigned short int word) {
    return word & 0x00ff;
}

unsigned char Cpu::getHighNibble(unsigned char byte) {
    return (byte & 0b11110000) >> 4;
}

unsigned char Cpu::getLowNibble(unsigned char byte) {
    return byte & 0b00001111;
}

unsigned short int Cpu::createAddress(unsigned char high, unsigned char low) {
    return (unsigned short int)(high << 8) + (unsigned short int)low;
}

void Cpu::sys_addr_func(unsigned char high, unsigned char low) {
    // not to be implemented in a modern emulator
}

void Cpu::cls_func(unsigned char high, unsigned char low) {
    for (int i = 0; i < 2048; i++) (*pixels)[i] = 0; // turning pixels "off"
}

void Cpu::ret_func(unsigned char high, unsigned char low) {
    PC = stack[SP];
    SP--;
}

void Cpu::jp_addr_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    PC = createAddress(highLowNibble, low);
}

void Cpu::call_addr_func(unsigned char high, unsigned char low) {
    SP++;
    stack[SP] = PC;
    unsigned char highLowNibble = getLowNibble(high);
    PC = createAddress(highLowNibble, low);
}

void Cpu::se_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    if (vx == low) PC += 0x04;
}

void Cpu::sne_vx_btye_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    if (vx != low) PC += 0x04;
}

void Cpu::se_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    unsigned char lowHighNibble = getHighNibble(low);
    unsigned char vy = registers[lowHighNibble];
    if (vx == vy) PC += 0x04;
}

void Cpu::ld_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    registers[highLowNibble] = low;
}

void Cpu::add_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    registers[highLowNibble] += low;
}

void Cpu::ld_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[lowHighNibble];
}

void Cpu::or_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] | registers[lowHighNibble];
}

void Cpu::and_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] & registers[lowHighNibble];
}

void Cpu::xor_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] ^ registers[lowHighNibble];
}

void Cpu::add_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    if (registers[highLowNibble] + registers[lowHighNibble] > 255) {
        registers[Vf] = 1;
        registers[highLowNibble] = (registers[highLowNibble] + registers[lowHighNibble]) & 0xff;
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[highLowNibble] + registers[lowHighNibble];
    }
}

void Cpu::sub_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    if (registers[highLowNibble] > registers[lowHighNibble]) {
        registers[Vf] = 1;
        registers[highLowNibble] = registers[highLowNibble] - registers[lowHighNibble];
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[highLowNibble] - registers[lowHighNibble];
    }
}

void Cpu::shr_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if ((registers[highLowNibble] & 0b00000001) == 1) {
        registers[Vf] = 1;
        registers[highLowNibble] = registers[highLowNibble] >> 1;
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[highLowNibble] >> 1;
    }
}

void Cpu::subn_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    if (registers[lowHighNibble] > registers[highLowNibble]) {
        registers[Vf] = 1;
        registers[highLowNibble] = registers[lowHighNibble] - registers[highLowNibble];
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[lowHighNibble] - registers[highLowNibble];
    }
}

void Cpu::shl_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if ((registers[highLowNibble] & 0b10000000) == 1) {
        registers[Vf] = 1;
        registers[highLowNibble] = registers[highLowNibble] << 1;
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[highLowNibble] << 1;
    }
}

void Cpu::sne_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    if (highLowNibble != lowHighNibble) PC += 0x04;
}

void Cpu::ld_i_addr_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    I = createAddress(highLowNibble, low);
}

void Cpu::jp_v0_addr_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned short int address = createAddress(highLowNibble, low) + registers[V0];
    PC = address;
}

void Cpu::rnd_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char randomNumber = rand() % 256;
    registers[highLowNibble] = (randomNumber & low);
}

void Cpu::drw_vx_vy_nibble_func(unsigned char high, unsigned char low) {
    for (int i = 0; i < 2048; i++) (*pixels)[i] = 0;
}

void Cpu::skp_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::sknp_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_vx_dt_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_vx_k_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_dt_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_st_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::add_i_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_f_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_b_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_i_vx_func(unsigned char high, unsigned char low) {

}

void Cpu::ld_vx_i_func(unsigned char high, unsigned char low) {

}