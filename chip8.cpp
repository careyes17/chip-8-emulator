#include "headers/chip8.h"
#include "headers/input.h"

void Chip8::update() {
    printf("3");

    // debug
    if (keyPressed(KEY_A)) printf("a");
    for (int i = 0; i < 2048; i++) (*pixels)[i] = rand() % 2;
}