//
// Created by Dorcy Agape on 09.10.24.
//

#include "Scarfy.hpp"

constexpr int jumpUpFrame = 3;
constexpr int jumpDownFrame = 4;
constexpr int leftFootFrame = 1;
constexpr int rightFootFrame = 4;



Scarfy::Scarfy(){
    image = LoadTexture("../Graphics/scarfy.png");
    footStepSound = LoadSound ("../Sounds/step-C.mp3");
    landingSound= LoadSound("../Sounds/step-A.mp3");

    numFrames = 6;
    frameWidth = image.width / 6;
    frameDelay = 5.0f / 60.0f;
    frameDelayCounter = 0;
    frameIndex = 0;
    frameRect = Rectangle{ 0.0f, 0.0f, static_cast<float>(frameWidth), static_cast<float>(image.height) } ;

    isOnGround = true;
}

Scarfy::~Scarfy() {
    UnloadTexture(image);
    UnloadSound(footStepSound);
    UnloadSound(landingSound);
}

void Scarfy::draw() {
    auto [x,y] = getUpperLeftPosition();
    if (facingRight) {
        frameRect.width = static_cast<float>(frameWidth);
    }else {
        frameRect.width = -static_cast<float>(frameWidth);
    }
    raylib::Rectangle destRect(x, y, static_cast<float>(frameWidth) , static_cast<float>(image.height));

    DrawTexturePro(image,frameRect,destRect,{0,0},0.0f,WHITE);
}

/*
 * Deals with the update of frames for the Scarfy Animation
 * @param bool onGround: lets us know if Scarfy is currently on the ground.
 *
 */

bool Scarfy::update(float elapsedTime, b2WorldId worldId) {
    CharacterActor::update(elapsedTime, worldId);
    b2Vec2 oldPos = getPosition();
    const auto &velocity = getVelocity();
    bool scarfyMoving = isMoving();

    if (!wasOnGround && isOnGround) {
        PlaySound(landingSound);
    }

    frameDelayCounter+= elapsedTime;

    if (frameDelayCounter > frameDelay) {
        frameDelayCounter = 0;
        if (scarfyMoving) {
            if (isOnGround) {
                frameIndex++;
                frameIndex %= numFrames;
                // After one frame, check actual position change
                TraceLog(LOG_INFO, "Velocity: %.2f m/s, Position: (%.2f, %.2f)",
                         velocity.x, oldPos.x, oldPos.y);
                if (frameIndex == leftFootFrame || frameIndex == rightFootFrame) {

                    PlaySound(footStepSound);
                }
            } else {
                if (velocity.y < 0.0f)
                    frameIndex = jumpUpFrame;
                else if (velocity.y > 0.0f)
                    frameIndex = jumpDownFrame;
            }
            //TODO: Implement a standing Fram (optional)
            frameRect.x = static_cast<float>(frameIndex) * static_cast<float>(frameWidth);
        }
    }

    wasOnGround = isOnGround;
    return true;
}


raylib::Rectangle Scarfy::getBoundingBox() {
    auto [x,y] = getUpperLeftPosition();
    return Rectangle{x, y, static_cast<float>(frameWidth), static_cast<float>(image.height)};
}
raylib::Vector2 Scarfy::getUpperLeftPosition() {
    const b2Vec2 &position = getPosition();
    const float xPos = position.x - (static_cast<float>(frameWidth) / 2.0f);
    const float yPos = position.y - (static_cast<float>(image.height) / 2.0f);
    return{xPos, yPos};
}
