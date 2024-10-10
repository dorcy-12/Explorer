#include "SoundEffects.hpp"

SoundEffects::SoundEffects() {
    footStep = LoadSound("../Sounds/step-C.mp3");
    landing =  LoadSound("../Sounds/step-A.mp3");
    isPlaying = false;
}
SoundEffects::~SoundEffects() {
    UnloadSound(footStep);
    UnloadSound(landing);
}

void SoundEffects::playStep() {
    PlaySound(footStep);
}
void SoundEffects::playLanding() {
    PlaySound(landing);
}





