#include "headers/chip8.h"
#include "headers/input.h"

#define SCREENWIDTH 960
#define SCREENHEIGHT 480
#define PIXELWIDTH 15
#define PIXELHEIGHT 15
#define CPUHZ 500
#define FPS 60

const key keyCodes[16] = {
    KEY_X, // 0
    KEY_1, // 1
    KEY_2, // 2
    KEY_3, // 3
    KEY_Q, // 4
    KEY_W, // 5
    KEY_E, // 6
    KEY_A, // 7
    KEY_S, // 8
    KEY_D, // 9
    KEY_Z, // A
    KEY_C, // B
    KEY_4, // C
    KEY_R, // D
    KEY_F, // E
    KEY_V // F
};

Chip8::Chip8(std::vector<char>* pixels, const char* filename):
    Engine(SCREENWIDTH, SCREENHEIGHT, PIXELWIDTH, PIXELHEIGHT, FPS, pixels),
    pixels(NULL),
    cpu(pixels, filename){
        cpuFPSCap.init(CPUHZ);
        this->pixels = pixels;
    };

void Chip8::update() {
    cpuFPSCap.updateTime();
    int cpuCycles = cpuFPSCap.getNumberOfUpdates();
    for (int i = 0; i < cpuCycles; i++) {
        // operations happen at 500 hz
        cpu.step();
    }

    // operations happen at 60 hz
    cpu.decrementSoundAndTime();
    if (!sound.isPlaying() && cpu.getST() > 0) sound.play();
    else if (sound.isPlaying() && cpu.getST() < 1) sound.stop();

    char key = -1;
    char keys[16];
    for (int i = 0; i < 16; i++) {
        if (keyPressed(keyCodes[i])) {
            key = i;
            keys[i] = 1;
        } else {
            keys[i] = 0;
        }
    }
    cpu.updateLastKeyPressed(key);
    cpu.updateKeysPressed(keys);
}