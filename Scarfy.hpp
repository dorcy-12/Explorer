#pragma once
#include "Actor.hpp"

class Scarfy : public Actor {
public:
    Scarfy();

    virtual ~Scarfy();

    virtual void draw();

    virtual void update(bool onGround);

    virtual Rectangle getBoundingBox();

    void goUp();

    void goDown();

    void goLeft() ;

    void goRight();

    void goNowhere();

    void interact();


private:
    Texture2D image;


    Sound footStepSound;
    Sound landingSound;

    unsigned numFrames;
    int frameWidth;
    int frameDelay;
    int frameDelayCounter;
    unsigned frameIndex;
    Rectangle frameRect;

    float jumpSpeed;
    float walkSpeed;

    bool isOnGround;
};



