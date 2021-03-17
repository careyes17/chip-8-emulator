#include "headers/chip8.h"
#include "headers/input.h"

void Chip8::update() {
    cpuFPSCap.updateTime();
    for (int i = 0; i < cpuFPSCap.getNumberOfUpdates(); i++) {
        // operations happen at 500 hz
    }

    // debug
    printf("3");
    if (keyPressed(KEY_A)) printf("a");
    for (int i = 0; i < 2048; i++) (*pixels)[i] = rand() % 2;
    cpu.decrementSoundAndTime();
}