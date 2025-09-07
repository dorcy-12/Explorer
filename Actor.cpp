
#include "Actor.hpp"
static const b2Vec2 zeroVector(0.0f, 0.0f);

Actor::Actor() = default;

Actor::~Actor() = default;

bool Actor::update(float elapsedTime) {
    return true;
}

void Actor::draw() {
}

void Actor::drawBoundingBox(const Color &color) {
}

raylib::Rectangle Actor::getBoundingBox() {
    const auto &[x, y] = getPosition();
    return {x, y};
}

b2Vec2 Actor::getPosition() const {
    if (b2Body_IsValid(physicsBodyId)) {
         return b2Body_GetPosition(physicsBodyId);
    }
    return zeroVector;
}

void Actor::setPosition(const b2Vec2 &position) const{
    if (b2Body_IsValid(physicsBodyId)) {
        b2Body_SetTransform (physicsBodyId, position, {0,0});
    }else {
        throw std::runtime_error("Can't set actor's position, because it has no physics body.");
    }
}

b2Vec2 Actor::getVelocity() const {
    if (b2Body_IsValid(physicsBodyId)) {
        return b2Body_GetLinearVelocity(physicsBodyId);
    }
    return zeroVector;
}

void Actor::setVelocity(const b2Vec2 &velocity) const {
    if (b2Body_IsValid(physicsBodyId)) {
        b2Body_SetLinearVelocity(physicsBodyId, velocity);
    } else {
        throw std::runtime_error("Can't set actor's velocity, because it has no physics body.");
    }
}

/**
 * Floating point errors mean that sometimes the velocity isn’t exactly zero even when the object is “standing still.”
 * Hence, we check if the squared length is above a certain threshold.
 * @return isCharacterMoving
 */
bool Actor::isMoving() const {
    auto [x, y] = getVelocity();
    const float lengthSquared = x * x + y * y;
    return lengthSquared > 10.0f * FLT_EPSILON;
}

void Actor::createPhysicsBody(b2WorldId world_id, const b2Vec2 &position, const b2Vec2 &velocity) {
}

void Actor::deletePhysicsBody() {
    if (b2Body_IsValid(physicsBodyId)) {
        b2DestroyBody(physicsBodyId);
        physicsBodyId = {0};
    }
}



