#include "headers/cpu.h"

void Cpu::step() {
    // takes next "clock cycle" step
}

void Cpu::decrementSoundAndTime() {
    // check if DT or ST are "active" and decrement them
    if (this->DT > 0) this->DT--;
    if (this->ST > 0) this->ST--;
}