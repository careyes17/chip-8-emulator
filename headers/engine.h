#pragma once
#include <vector>
#include "video.h"
#include "sound.h"

class Engine : public Video {
    public:
        Sound sound;
        Engine(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, double fps, std::vector<char>* pixels);
        ~Engine();
};