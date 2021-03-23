#pragma once

class Sound {
private:
    SDL_AudioSpec want;
    SDL_AudioSpec have;
    int sample_nr;
    bool playing;
public:
    Sound();
    ~Sound();
    void play();
    void stop();
    bool isPlaying();
};