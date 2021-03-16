#include "headers/cpu.h"

void Cpu::step() {
    // takes next "clock cycle" step
}

void Cpu::decrementSoundAndTime() {
    // check if DT or ST are "active" and decrement them
    if (this->DT > 0) this->DT--;
    if (this->ST > 0) this->ST--;
}

void Cpu::instruction(unsigned char high, unsigned char low) {

}

unsigned char Cpu::getHighNibble(unsigned char byte) {
    return (byte & 0b11110000) >> 4;
}

unsigned char Cpu::getLowNibble(unsigned char byte) {
    return byte & 0b00001111;
}