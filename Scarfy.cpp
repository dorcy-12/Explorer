//
// Created by Dorcy Agape on 09.10.24.
//

#include "Scarfy.hpp"

const int jumpUpFrame = 3;
const int jumpDownFrame = 4;
const int leftFootFrame = 1;
const int rightFootFrame = 4;


Scarfy::Scarfy() {
    image = LoadTexture("../Graphics/scarfy.png");
    footStepSound = LoadSound("../Sounds/step-C.mp3");
    landingSound =  LoadSound("../Sounds/step-A.mp3");

    numFrames = 6;
    frameWidth = image.width / 6;
    frameDelay = 5;
    frameDelayCounter = 0;
    frameIndex = 0;
    frameRect = Rectangle{ 0.0f, 0.0f, static_cast<float>(frameWidth), static_cast<float>(image.height) } ;

    walkSpeed = 10;
    jumpSpeed = 2 * walkSpeed;

    isOnGround = true;
}

Scarfy::~Scarfy() {
    UnloadTexture(image);
    UnloadSound(footStepSound);
    UnloadSound(landingSound);
}

void Scarfy::draw() {
    DrawTextureRec(image,frameRect,position,WHITE);
    DrawCircle(position.x,position.y,5.0f,RED); // remove
}


/*
 * Deals with the update of frames for the Scarfy Animation
 * @param bool onGround: lets us know if Scarfy is currently on the ground.
 *
 */

void Scarfy::update(bool onGround) {
    bool isMoving = velocity.x != 0.0f || velocity.y != 0.0f;

    bool wasOnGround = isOnGround;
    isOnGround = onGround;

    if (!wasOnGround && isOnGround) {
        PlaySound(landingSound);
    }

    frameDelayCounter++;

    if (frameDelayCounter >= frameDelay) {
        frameDelayCounter = 0;
        if (isMoving) {
            if (isOnGround) {
                frameIndex++;
                frameIndex %= numFrames;

                if (frameIndex == leftFootFrame || frameIndex == rightFootFrame) {
                    PlaySound(footStepSound);
                }
            }
            else {
                if (velocity.y < 0.0f) frameIndex=jumpUpFrame;
                else if (velocity.y > 0.0f) frameIndex=jumpDownFrame;
            }
            frameRect.x = static_cast<float>(frameIndex)* static_cast<float>(frameWidth);
        }
        else {

        }
    }

}


Rectangle Scarfy::getBoundingBox() {
    return Rectangle{position.x, position.y, static_cast<float>(frameWidth), static_cast<float>(image.height)};
}

void Scarfy::goUp() {
    if (isOnGround) {
        velocity.y -= jumpSpeed;
    }
}

void Scarfy::goLeft() {
    if(isOnGround) {
        velocity.x = -walkSpeed;
        if (frameRect.width > 0) {  // player facing left but now turning right
            frameRect.width *= -1;
        }
    }
}

void Scarfy::goRight() {
    if(isOnGround) {
        velocity.x = walkSpeed;
        if (frameRect.width < 0) {  // player facing left but now turning right
            frameRect.width *= -1;
        }
    }
}

void Scarfy::goDown() {

}

void Scarfy::goNowhere() {
    if (isOnGround) {
        velocity.x = 0;

    }
}

void Scarfy::interact() {
    Actor::interact();
}

