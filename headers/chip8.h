#pragma once
#include "headers/video.h"
#include "headers/sound.h"
#include "headers/cpu.h"

class Chip8 : public Video{
    private:
        Cpu cpu;
        Sound sound;
    public:
        Chip8();
        ~Chip8();
};