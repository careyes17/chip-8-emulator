#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "headers/video.h"

Video::Video(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, double fps, std::vector<char>* pixels):
    window(NULL),
    renderer(NULL),
    fps(fps),
    pixels(NULL),
    running(true) {
        props.screenWidth = screenWidth;
        props.screenHeight = screenHeight;
        props.pixelWidth = pixelWidth;
        props.pixelHeight = pixelHeight;
        props.rows = screenHeight/pixelHeight;
        props.cols = screenWidth/pixelWidth;

        this->pixels = pixels;
    }

Video::~Video() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Video::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        // create window
        window = SDL_CreateWindow(
            "Game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            props.screenWidth,
            props.screenHeight,
            SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
            running = false;
        }

        // create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if ( renderer == NULL ) {
            printf("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
            running = false;
        }
    }
    return;
}

void Video::pollInput() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev) != 0) {
        switch(ev.type){
            case SDL_KEYDOWN:
                switch( ev.key.keysym.sym ){
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
    }
    return;
}

void Video::clear() {
    // clears screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void Video::render() {
    // draw pixels to renderer
    clear();
    drawPixels();
    // Render the rect to the screen
    SDL_RenderPresent(renderer);
    return;
}

void Video::loop() {
    if (!running) return;

    // init framerate capping
    // handles all of the timing related to capping FPS
    FPSCapped gameFPSCap;
    gameFPSCap.init(fps);
    
    // init uncapped FPS
    // this serves as a measure of how many iterations
    // happen in the game loop
    FPSUncapped gameFPS;
    gameFPS.init();

    update(); // initial update (prevents garbage from being shown)

    // infinite loop: the game loop
    while(running) {
        // update capped FPS time
        gameFPSCap.updateTime();

        pollInput();

        for (int i = 0; i < gameFPSCap.getNumberOfUpdates(); i++) {
            update(); // update game logic
        }

        render(); // render to the screen

        // increment uncapped FPS
        gameFPS.attemptPrint();
    }
    return;
}

void Video::updatePixel(int x, int y, bool on) {
    (*pixels)[(x * props.cols) + y] = on;
    return;
}

void Video::drawPixels() {
    for (int i = 0; i < props.rows * props.cols; i++) {
        
        int y = i / props.cols;
        int x = i - props.cols*y;

        // adjusting for pixel size
        y *= props.pixelWidth;
        x *= props.pixelHeight;

        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = props.pixelWidth;
        r.h = props.pixelHeight;

        // std::cout << x << " " << y << std::endl;

        int color = (*pixels)[i] ? 255 : 0;

        // Render rect
        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderFillRect(renderer, &r);
    }
}