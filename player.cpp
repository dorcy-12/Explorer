#include "player.hpp"

Player::Player() {
    image = LoadTexture("../Graphics/scarfy.png");
    velocity = { 0.0f, 0.0f };
    ground = { 0.0f, 300.0f };
    position = { 0.0f, ground.y - static_cast<float>(image.height) };

    frameRec = { 0.0f, 0.0f, (float)image.width/6, (float)image.height };
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 10;

    gravity = 0.5f;
    jumping = false;
    jumpForce = -12.0f;
}

Player::~Player() {
    UnloadTexture(image);
}
void Player::Draw() {
    DrawTextureRec(image, frameRec, position, WHITE);
    DrawLine(0,static_cast<int>(ground.y), GetScreenWidth(), static_cast<int>(ground.y), BLACK);
}

void Player::Update() {

    framesCounter++;
    if (IsKeyDown(KEY_UP) && onGround()) {
        if (framesCounter >= (60/framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            jumping = true;
            velocity.y = jumpForce;
        }
    }
    if (!onGround() || jumping) {
        position.y += velocity.y;
        velocity.y += gravity;  // Gravity decreases the upward velocity until the player falls

        // Stop falling when the player reaches the ground
        if (position.y + (float)image.height >= ground.y) {
            position.y = ground.y - static_cast<float>(image.height);
            velocity.y = 0.0f;
            jumping = false;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) frameUpdate(KEY_RIGHT);

    if (IsKeyDown(KEY_LEFT))  frameUpdate(KEY_LEFT);

}

void Player::frameUpdate(int dir) {
    if (framesCounter >= (60/framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > 5) currentFrame = 0;
        frameRec.x = static_cast<float>(currentFrame)*static_cast<float>(image.width)/6;

        if (dir == KEY_RIGHT ) {
            if (frameRec.width < 0) {  // player facing left but now turning right
                frameRec.width *= -1;;
            }
            position.x+= static_cast<float>(framesSpeed) * 2;
        }
        if (dir == KEY_LEFT ) {
            if (frameRec.width > 0) {  // player facing right but now turning right
                frameRec.width *= -1;;
            }
            position.x-= static_cast<float>(framesSpeed) * 2;
        }

    }
}

bool Player::onGround() {
    return position.y + static_cast<float>(image.height) >= ground.y;
}
