#pragma once
#include <vector>
#include "engine.h"
#include "cpu.h"

class Chip8 : public Engine {
    private:
        std::vector<char>* pixels;
        Cpu cpu;
        FPSCapped cpuFPSCap;
    public:
        Chip8(std::vector<char>* pixels);
        ~Chip8(){};
        void update() override;
};