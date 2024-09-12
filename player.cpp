#include "player.hpp"

Player::Player() {
    image = LoadTexture("../Graphics/scarfy.png");
    image.height /= 2;
    image.width /= 2;
    velocity = { 10.0f, 0.0f };
    ground = { 0.0f, 300.0f };
    position = { 0.0f, ground.y - static_cast<float>(image.height) };
    speed = 20.0f;

    frameRec = { 0.0f, 0.0f, static_cast<float>(image.width)/6, static_cast<float>(image.height) };
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 10;

    gravity = 1.0f;
    jumping = false;
    jumpForce = -12.0f;
}

Player::~Player() {
    UnloadTexture(image);
}
void Player::Draw() const {
    DrawTextureRec(image, frameRec, position, WHITE);
    DrawLine(0,static_cast<int>(ground.y), GetScreenWidth(), static_cast<int>(ground.y), BLACK);
    DrawText(to_string(velocity.x).c_str(),20,20,30,BLACK);
    DrawText(to_string(velocity.y).c_str(),20,50,30,BLACK);
}

void Player::Update() {
    if (onGround()) {
        if (IsKeyDown(KEY_UP)) {
            jumping = true;
            velocity.y = jumpForce;
        }

        if (IsKeyDown(KEY_RIGHT)) {
            if (frameRec.width < 0) {  // player facing left but now turning right
                frameRec.width *= -1;
            }
            velocity.x = speed;
        }else if (IsKeyDown(KEY_LEFT)) {
            if (frameRec.width > 0) {  // player facing right but now turning right
                frameRec.width *= -1;;
            }
            velocity.x = -speed;
        }else {
            velocity.x = 0.0f;
        }
    }
    else if (!onGround() || jumping) {
        position.y += velocity.y;
        velocity.y += gravity;  // Gravity decreases the upward velocity until the player falls

        // Stop falling when the player reaches the ground
        if (position.y + static_cast<float>(image.height) >= ground.y) {
            position.y = ground.y - static_cast<float>(image.height);
            velocity.y = 0.0f;
            jumping = false;
        }
    }
    frameUpdate();

}

void Player::frameUpdate() {
    framesCounter++;
    if (framesCounter >= (60/framesSpeed))
    {
        framesCounter = 0;
        if (velocity.y<0.0f) {
            currentFrame=2;
            position.y += velocity.y;
            position.x += velocity.x;
        }
        else if (velocity.x != 0.0f || velocity.y > 0.0f) {
            currentFrame++;
            position.y += velocity.y;
            position.x += velocity.x;
        }
        if (currentFrame > 5) currentFrame = 0;
        frameRec.x = static_cast<float>(currentFrame)*static_cast<float>(image.width)/6;
    }
}

bool Player::onGround() const {
    return position.y + static_cast<float>(image.height) >= ground.y;
}
