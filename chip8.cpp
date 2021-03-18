#include "headers/chip8.h"
#include "headers/input.h"

void Chip8::update() {
    cpuFPSCap.updateTime();
    int cpuCycles = cpuFPSCap.getNumberOfUpdates();
    for (int i = 0; i < cpuCycles; i++) {
        // operations happen at 500 hz

        // debug
        // cpu.cls_func(0x12,0x34);
        // cpu.getHighByte(0x04ff);
        // cpu.getLowByte(0xff0a);
    }

    // debug
    printf("3");
    if (keyPressed(KEY_A)) printf("a");
    for (int i = 0; i < 2048; i++) (*pixels)[i] = rand() % 2;
    cpu.decrementSoundAndTime();
}