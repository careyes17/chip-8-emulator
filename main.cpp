#include "headers/chip8.h"

#define SCREENWIDTH 960
#define SCREENHEIGHT 480
#define PIXELWIDTH 15
#define PIXELHEIGHT 15
#define NUMOFTOTALPIXELS (SCREENWIDTH/PIXELWIDTH)*(SCREENHEIGHT/PIXELHEIGHT)

int main(int argc, char* args[]) {
    Pixels pixels;
    pixels.init(NUMOFTOTALPIXELS);

    // initialize game object, inheriting the game engine and including its own game logic
    Chip8 chip8(pixels.pixels, args[1]);
    chip8.init();
    chip8.loop(); // inifinite game loop

    pixels.destroy();

    return EXIT_SUCCESS;
}
