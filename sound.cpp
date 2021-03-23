#include <SDL2/SDL.h>
#include "headers/sound.h"
#include "math.h"

const int SAMPLE_RATE = 44100;
const int AMPLITUDE = 3000;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16*)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int &sample_nr(*(int*)user_data);

    for(int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * pow(-1, floor(800*time/2.0f)));
    }
}

Sound::Sound(): sample_nr(0) {
    want.freq = SAMPLE_RATE; // number of samples per second
    want.format = AUDIO_S16SYS; // sample type (here: signed short i.e. 16 bit)
    want.channels = 1; // only one channel
    want.samples = 2048; // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr; // counter, keeping track of current sample number

    if(SDL_OpenAudio(&want, &have) != 0) SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
    if(want.format != have.format) SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");
}

Sound::~Sound() {
    SDL_CloseAudio();
}

void Sound::play() {
    this->playing = true;
    SDL_PauseAudio(0); // start playing sound
}

void Sound::stop() {
    this->playing = false;
    SDL_PauseAudio(1); // stop playing sound
}

bool Sound::isPlaying() {
    return this->playing;
}