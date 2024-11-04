#pragma once
#include "Command.hpp"
#include "raylib-cpp-master/include/raylib-cpp.hpp"

class Actor : public Command {
public:
    Actor();
    virtual ~Actor();
    virtual void draw();
    virtual bool update(bool onGround);
    virtual Rectangle getBoundingBox();


public:
    raylib::Vector2 position{};
    raylib::Vector2 velocity{};
};
