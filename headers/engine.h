#pragma once
#include <vector>
#include "video.h"
#include "sound.h"

class Engine : public Video {
    private:
        Sound sound;
    public:
        Engine(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, double fps, std::vector<char>* pixels);
        ~Engine();
};