#pragma once
#include <raylib.h>

class SoundEffects {
public:
    SoundEffects();
    ~SoundEffects();
    void playStep();
    void playLanding();
private:
    Sound footStep{};
    Sound landing{};
    bool isPlaying{};
};


