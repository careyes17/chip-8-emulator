#pragma once

class Sound {
private:
    SDL_AudioSpec want;
    SDL_AudioSpec have;
    int sample_nr;
public:
    Sound();
    ~Sound();
    void play();
    void stop();
};