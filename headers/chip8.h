#pragma once
#include "engine.h"
#include "cpu.h"

#define SCREENWIDTH 960
#define SCREENHEIGHT 480
#define PIXELWIDTH 15
#define PIXELHEIGHT 15
#define NUMOFTOTALPIXELS (SCREENWIDTH/PIXELWIDTH)*(SCREENHEIGHT/PIXELHEIGHT)
#define FPS 60.0

class Chip8 : public Engine {
    private:
        Cpu cpu;
    public:
        Chip8(std::vector<char>& pixels):
            Engine(SCREENWIDTH, SCREENHEIGHT, PIXELWIDTH, PIXELHEIGHT, FPS, pixels){};
        ~Chip8(){};
        void update() override;
};