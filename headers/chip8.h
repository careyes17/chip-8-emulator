#pragma once
#include "headers/video.h"
#include "headers/sound.h"
#include "headers/cpu.h"

class Chip8 {
    private:
        Cpu cpu;
        Video video;
        Sound sound;
    public:
        Chip8();
        ~Chip8();
};