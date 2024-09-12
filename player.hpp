#pragma once
#include <raylib.h>
#include <string>

using namespace std;

class Player {
public:
    Player();
    ~Player();
    void Draw();
    void Update();
private:
    Texture2D image{};
    Vector2 position{};
    Rectangle frameRec{};
    Vector2 ground{};
    Vector2 velocity{};

    int framesSpeed;
    int currentFrame;
    int framesCounter;
    void frameUpdate(int dir);
    bool onGround();
    float gravity;
    bool jumping;
    float jumpForce;

};




