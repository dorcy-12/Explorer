
#include "Actor.hpp"

Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::draw() {
    DrawText("Override the Draw Method",20,20,20,RED);
}

void Actor::update(bool onGround) {
}

Rectangle Actor::getBoundingBox() {
    return Rectangle{position.x, position.y};
}
