#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

struct Pixels {
    std::vector<char> pixels;
    void init(int numberOfTotalPixels) {
        // resizing pixels vector to appropriate size
        pixels.resize(0);
        pixels.resize(numberOfTotalPixels, 1);

        // setting random test values in pixels
        for (int i = 0; i < numberOfTotalPixels; i++) pixels[i] = rand() % 2;
    }
};

struct VideoProps {
    int screenWidth;
    int screenHeight;
    int pixelWidth;
    int pixelHeight;
    int rows;
    int cols;
};

struct FPSUncapped {
    int frames;
    int lastTime;
    void printFPS() {
        std::cout << "FPS: " << frames << std::endl;
    }
    void init() {
        frames = 0;
        lastTime = time(NULL);
    }
    void attemptPrint() {
        frames++;
        int currentTime = time(NULL);
        if (currentTime - lastTime >= 1) {
            lastTime = currentTime;
            printFPS();
            frames = 0;
        }
    }
};

struct FPSCapped {
    double oldTime;
    double accumulator;
    double currentTime;
    double deltaTime;
    int capValue;
    void init(int fps) {
        oldTime = SDL_GetTicks();
        accumulator = 0.0;
        currentTime = 0.0;
        deltaTime = 0.0;
        capValue = fps;
    }
    void updateTime() {
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - oldTime;
        oldTime = currentTime;
        accumulator += deltaTime;
    }
    int getNumberOfUpdates() {
        int numberOfUpdates = 0;
        while (accumulator > (1000.0/capValue)) {
            numberOfUpdates++;
            accumulator -= (1000.0/capValue);
            if(accumulator < 0) accumulator = 0;
        }
        return numberOfUpdates;
    }
};

class Video {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        VideoProps props;
        
        double fps;
        std::vector<char> pixels;
        
        bool running;

    public:
        Video(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, double fps, std::vector<char>& pixels);
        ~Video();
        void init();
        void pollInput();
        virtual void update(){};
        void render();
        void loop();

        void updatePixel(int x, int y, bool on);
        void clear();
        void drawPixels();
};