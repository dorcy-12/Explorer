#pragma once
#include "Actor.hpp"
#include "CharacterActor.hpp"
#include "raylib-cpp.hpp"


class Scarfy : public CharacterActor {
public:
    Scarfy();

    virtual ~Scarfy();

    void draw() override;

    bool update(float elapsedTime, b2WorldId worldId) override;

    raylib::Rectangle getBoundingBox() override;


private:

    raylib::Vector2 getUpperLeftPosition();
    Texture2D image;
    Sound footStepSound;
    Sound landingSound;

    unsigned numFrames;
    int frameWidth;
    float frameDelay;
    float frameDelayCounter;
    unsigned frameIndex;
    Rectangle frameRect;

    bool isOnGround;
};



