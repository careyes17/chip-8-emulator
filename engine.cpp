#include "headers/engine.h"
#include <vector>

Engine::Engine(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, double fps, std::vector<char>& pixels):
    Video(screenWidth, screenHeight, pixelWidth, pixelHeight, fps, pixels),
    sound(){};

Engine::~Engine(){};