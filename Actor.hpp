#pragma once
#include "Command.hpp"
#include "raylib.h"

class Actor : public Command {
public:
    Actor();
    ~Actor();
    virtual void draw();
    virtual bool update(bool onGround);
    virtual Rectangle getBoundingBox();


public:
    Vector2 position{};
    Vector2 velocity{};
};
