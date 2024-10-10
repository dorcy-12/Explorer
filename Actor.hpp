#pragma once
#include "Command.h"
#include "raylib.h"

class Actor : public Command {
public:
    Actor();
    ~Actor();
    virtual void draw();
    virtual void update(bool onGround);
    virtual Rectangle getBoundingBox();


public:
    Vector2 position{};
    Vector2 velocity{};
};
