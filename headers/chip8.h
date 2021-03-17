#pragma once
#include <vector>
#include "engine.h"
#include "cpu.h"

#define SCREENWIDTH 960
#define SCREENHEIGHT 480
#define PIXELWIDTH 15
#define PIXELHEIGHT 15
#define CPUHZ 500
#define FPS 60

class Chip8 : public Engine {
    private:
        std::vector<char>* pixels;
        char keysPressed[16];
        Cpu cpu;
        FPSCapped cpuFPSCap;
    public:
        Chip8(std::vector<char>* pixels):
            Engine(SCREENWIDTH, SCREENHEIGHT, PIXELWIDTH, PIXELHEIGHT, FPS, pixels),
            pixels(NULL),
            cpu(){
                cpuFPSCap.init(CPUHZ);
                this->pixels = pixels;
            };
        ~Chip8(){};
        void update() override;
};