#include "headers/cpu.h"
#include <fstream>
#include <vector>

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

    loadGameRomIntoRam("roms/.ch8");

    DT = 0;
    ST = 0;
    I = 0;
    SP = -1;

    for (int i = 0; i < 16; i++) {
        registers[i] = 0;
        stack[i] = 0;
        keysPressed[i] = 0;
    }
};

Cpu::~Cpu(){};

void Cpu::step() {
    // takes next "clock cycle" step
    
    // get next instruction from PC
    Instruction nextInstruction = getInstruction(ram[PC], ram[PC+1]);

    // execute instruction
    executeInstruction(nextInstruction, ram[PC], ram[PC+1]);
    
    // keypress debug
    // for (int i = 0; i < 16; i++) printf("%d, ", keysPressed[i]);
    // printf("%d", lastKeyPressed);
    // printf("\n"); 
    
    // PC instruction debug
    // printf("%02x%02x\n", ram[PC], ram[PC+1]);
}

void Cpu::decrementSoundAndTime() {
    // check if DT or ST are "active" and decrement them
    if (this->DT > 0) this->DT--;
    if (this->ST > 0) this->ST--;
}

static std::vector<unsigned char> readRom(const char* filename)
{
    // open the file
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data
    std::vector<unsigned char> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

void Cpu::loadGameRomIntoRam(const char* filename) {
    std::vector<unsigned char> gameRom = readRom(filename);
    int sizeOfGameRom = gameRom.size();
    for (int i = 0; i < sizeOfGameRom; i++) { // 16 characters
        const char* s = i % 10 == 0 ? "\n" : " ";
        printf("%02x%s", gameRom[i], s);
        ram[512+i] = gameRom[i];
    }
    PC = 512; // 200 hex
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
    switch(instruction) {
        case ERROR: // an invalid instruction is given
            break;
        case SYS_ADDR: // 0nnn - SYS addr
            sys_addr_func(high, low);
            break;
        case CLS: // 00E0 - CLS
            cls_func(high, low);
            break;
        case RET: // 00EE - RET
            ret_func(high, low);
            break;
        case JP_ADDR: // 1nnn - JP addr
            jp_addr_func(high, low);
            break;
        case CALL_ADDR: // 2nnn - CALL addr
            call_addr_func(high, low);
            break;
        case SE_VX_BYTE: // 3xkk - SE Vx, byte
            se_vx_byte_func(high, low);
            break;
        case SNE_VX_BYTE: // 4xkk - SNE Vx, byte
            sne_vx_btye_func(high, low);
            break;
        case SE_VX_VY: // 5xy0 - SE Vx, Vy
            se_vx_vy_func(high, low);
            break;
        case LD_VX_BYTE: // 6xkk - LD Vx, byte
            ld_vx_byte_func(high, low);
            break;
        case ADD_VX_BYTE: // 7xkk - ADD Vx, byte
            add_vx_byte_func(high, low);
            break;
        case LD_VX_VY: // 8xy0 - LD Vx, Vy
            ld_vx_vy_func(high, low);
            break;
        case OR_VX_VY: // 8xy1 - OR Vx, Vy
            or_vx_vy_func(high, low);
            break;
        case AND_VX_VY: // 8xy2 - AND Vx, Vy
            and_vx_vy_func(high, low);
            break;
        case XOR_VX_VY: // 8xy3 - XOR Vx, Vy
            xor_vx_vy_func(high, low);
            break;
        case ADD_VX_VY: // 8xy4 - ADD Vx, Vy
            add_vx_vy_func(high, low);
            break;
        case SUB_VX_VY: // 8xy5 - SUB Vx, Vy
            sub_vx_vy_func(high, low);
            break;
        case SHR_VX_VY: // 8xy6 - SHR Vx {, Vy}
            shr_vx_vy_func(high, low);
            break;
        case SUBN_VX_VY: // 8xy7 - SUBN Vx, Vy
            subn_vx_vy_func(high, low);
            break;
        case SHL_VX_VY: // 8xyE - SHL Vx {, Vy}
            shl_vx_vy_func(high, low); 
            break;
        case SNE_VX_VY: // 9xy0 - SNE Vx, Vy
            sne_vx_vy_func(high, low);
            break;
        case LD_I_ADDR: // Annn - LD I, addr
            ld_i_addr_func(high, low);
            break;
        case JP_V0_ADDR: // Bnnn - JP V0, addr
            jp_v0_addr_func(high, low);
            break;
        case RND_VX_BYTE: // Cxkk - RND Vx, byte
            rnd_vx_byte_func(high, low);
            break;
        case DRW_VX_VY_NIBBLE: // Dxyn - DRW Vx, Vy, nibble
            drw_vx_vy_nibble_func(high, low);
            break;
        case SKP_VX: // Ex9E - SKP Vx
            skp_vx_func(high, low);
            break;
        case SKNP_VX: // ExA1 - SKNP Vx
            sknp_vx_func(high, low);
            break;
        case LD_VX_DT: // Fx07 - LD Vx, DT
            ld_vx_dt_func(high, low);
            break;
        case LD_VX_K: // Fx0A - LD Vx, K
            ld_vx_k_func(high, low);
            break;
        case LD_DT_VX: // Fx15 - LD DT, Vx
            ld_dt_vx_func(high, low);
            break;
        case LD_ST_VX: // Fx18 - LD ST, Vx
            ld_st_vx_func(high, low);
            break;
        case ADD_I_VX: // Fx1E - ADD I, Vx
            add_i_vx_func(high, low);
            break;
        case LD_F_VX: // Fx29 - LD F, Vx
            ld_f_vx_func(high, low);
            break;
        case LD_B_VX: // Fx33 - LD B, Vx
            ld_b_vx_func(high, low);
            break;
        case LD_I_VX: // Fx55 - LD [I], Vx
            ld_i_vx_func(high, low);
            break;
        case LD_VX_I: // Fx65 - LD Vx, [I]
            ld_vx_i_func(high, low);
            break;
        default:
            break;
    }
}

static unsigned char correctXCoord(unsigned char x, int i) {
    if (!(x + i > 31)) return x + i;
    else return (x + i) - 32;
}

static unsigned char correctYCoord(unsigned char y, int j) {
    if (!(y + j > 63)) return y + j;
    else return (y + j) - 64;
}

unsigned char Cpu::getST() {
    return this->ST;
}

char Cpu::getPixelAtCoord(unsigned char x, unsigned char y) {
    return (*pixels)[((x * 64)+y)];
}

void Cpu::setPixelAtCoord(unsigned char x, unsigned char y, char value) {
    (*pixels)[((x * 64)+y)] = value;
}

void Cpu::updateKeysPressed(char keys[16]) {
    for (int i = 0; i < 16; i++) {
        keysPressed[i] = keys[i];
    }
}

void Cpu::updateLastKeyPressed(char key) {
    previousKeyPressed = lastKeyPressed;
    lastKeyPressed = key;
}

unsigned char Cpu::getHighByte(unsigned char word) {
    return word >> 4;
}

unsigned char Cpu::getLowByte(unsigned char word) {
    return word & 0x0f;
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

// instruction implementations

void Cpu::sys_addr_func(unsigned char high, unsigned char low) {
    // not to be implemented in a modern emulator
}

void Cpu::cls_func(unsigned char high, unsigned char low) {
    for (int i = 0; i < 2048; i++) (*pixels)[i] = 0; // turning pixels "off"
    PC += 0x02;
}

void Cpu::ret_func(unsigned char high, unsigned char low) {
    PC = stack[SP];
    stack[SP] = 0;
    SP--;
}

void Cpu::jp_addr_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    PC = createAddress(highLowNibble, low);
}

void Cpu::call_addr_func(unsigned char high, unsigned char low) {
    SP++;
    stack[SP] = PC + 0x02;
    unsigned char highLowNibble = getLowNibble(high);
    PC = createAddress(highLowNibble, low);
}

void Cpu::se_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    if (vx == low) PC += 0x04;
    else PC += 0x02;
}

void Cpu::sne_vx_btye_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    if (vx != low) PC += 0x04;
    else PC += 0x02;
}

void Cpu::se_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char vx = registers[highLowNibble];
    unsigned char lowHighNibble = getHighNibble(low);
    unsigned char vy = registers[lowHighNibble];
    if (vx == vy) PC += 0x04;
    else PC += 0x02;
}

void Cpu::ld_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    registers[highLowNibble] = low;
    PC += 0x02;
}

void Cpu::add_vx_byte_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    registers[highLowNibble] += low;
    PC += 0x02;
}

void Cpu::ld_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[lowHighNibble];
    PC += 0x02;
}

void Cpu::or_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] | registers[lowHighNibble];
    PC += 0x02;
}

void Cpu::and_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] & registers[lowHighNibble];
    PC += 0x02;
}

void Cpu::xor_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    registers[highLowNibble] = registers[highLowNibble] ^ registers[lowHighNibble];
    PC += 0x02;
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
    PC += 0x02;
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
    PC += 0x02;
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
    PC += 0x02;
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
    PC += 0x02;
}

void Cpu::shl_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if ((registers[highLowNibble] & 0b10000000) == 128) {
        registers[Vf] = 1;
        registers[highLowNibble] = registers[highLowNibble] << 1;
    } else {
        registers[Vf] = 0;
        registers[highLowNibble] = registers[highLowNibble] << 1;
    }
    PC += 0x02;
}

void Cpu::sne_vx_vy_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    unsigned char lowHighNibble = getHighNibble(low);
    if (registers[highLowNibble] != registers[lowHighNibble]) PC += 0x04;
    else PC += 0x02;
}

void Cpu::ld_i_addr_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    I = createAddress(highLowNibble, low);
    PC += 0x02;
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
    PC += 0x02;
}

void Cpu::drw_vx_vy_nibble_func(unsigned char high, unsigned char low) {
    unsigned char ycoordinate;
    unsigned char xcoordinate;
    unsigned char numberOfBytesToRead = getLowNibble(low);
    unsigned char bitMask[8] = {
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001
    };
    bool pixelUnset = 0;
    for (int i = 0; i < numberOfBytesToRead; i++) {
        unsigned char currentByte = ram[I+i];
        for (int j = 0; j < 8; j++) {
            ycoordinate = registers[getLowByte(high)];
            xcoordinate = registers[getHighByte(low)];
            xcoordinate = correctXCoord(xcoordinate, i);
            ycoordinate = correctYCoord(ycoordinate, j);
            unsigned char maskedValue = (currentByte & bitMask[j]) > 0 ? 1 : 0;
            if (getPixelAtCoord(xcoordinate, ycoordinate) == 1 && maskedValue == 1) {
                pixelUnset = 1;
                setPixelAtCoord(xcoordinate, ycoordinate, 0);
            } else if (getPixelAtCoord(xcoordinate, ycoordinate) == 0 && maskedValue == 0) {
                setPixelAtCoord(xcoordinate, ycoordinate, 0);
            } else setPixelAtCoord(xcoordinate, ycoordinate, 1);
        }
    }
    registers[Vf] = pixelUnset;
    PC += 0x02;
}

void Cpu::skp_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if (keysPressed[registers[highLowNibble]] == 1) PC += 0x04;
    else PC += 0x02;
}

void Cpu::sknp_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if (keysPressed[registers[highLowNibble]] == 0) PC += 0x04;
    else PC += 0x02;
}

void Cpu::ld_vx_dt_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    registers[highLowNibble] = DT;
    PC += 0x02;
}

void Cpu::ld_vx_k_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    if (previousKeyPressed == -1 && lastKeyPressed != -1) {
        registers[highLowNibble] = lastKeyPressed;
        updateLastKeyPressed(lastKeyPressed);
        PC += 0x02;
    }
}

void Cpu::ld_dt_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    DT = registers[highLowNibble];
    PC += 0x02;
}

void Cpu::ld_st_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    ST = registers[highLowNibble];
    PC += 0x02;
}

void Cpu::add_i_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    I += registers[highLowNibble];
    PC += 0x02;
}

void Cpu::ld_f_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    I = registers[highLowNibble] * 5;
    PC += 0x02;
}

void Cpu::ld_b_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    ram[I] = (registers[highLowNibble]/100) % 10;
    ram[I+1] = (registers[highLowNibble]/10) % 10;
    ram[I+2] = registers[highLowNibble] % 10;
    PC += 0x02;
}

void Cpu::ld_i_vx_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    for (int i = 0; i <= highLowNibble; i++) ram[I+i] = registers[i];
    PC += 0x02;
}

void Cpu::ld_vx_i_func(unsigned char high, unsigned char low) {
    unsigned char highLowNibble = getLowNibble(high);
    for (int i = 0; i <= highLowNibble; i++) registers[i] = ram[I+i];
    PC += 0x02;
}