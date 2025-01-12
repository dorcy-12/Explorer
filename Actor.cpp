
#include "Actor.hpp"

Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::draw() {
    DrawText("Override the Draw Method",20,20,20,RED);
}

bool Actor::update(bool onGround) {
    return true;
}

Rectangle Actor::getBoundingBox() {
    return Rectangle{position.x, position.y};
}

