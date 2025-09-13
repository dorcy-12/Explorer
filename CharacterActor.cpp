//
// Created by Dorcy Agape on 25.08.25.
//

#include "CharacterActor.hpp"


#define MOVING_FRICTION 0.2f
#define STANDING_FRICTION 1.0f

CharacterActor::CharacterActor() {
}

CharacterActor::~CharacterActor() {
}

void CharacterActor::createPhysicsBody(b2WorldId world_id,  const b2Vec2 &position,
    const b2Vec2 &velocity) {
    raylib::Rectangle boundingBox = getBoundingBox();

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.linearVelocity = velocity;
    float halfWidth = boundingBox.width / 2.0f;
    float halfHeight = boundingBox.height / 2.0f;

    //TODO: Friction Callback

    b2BodyId bodyId = b2CreateBody(world_id, &bodyDef);

    float torsoHeight = boundingBox.height - halfWidth; //?
    float torsoHalfHeight = torsoHeight / 2.0f;

    b2Capsule capsule; //TODO: Test if this works
    capsule.center1 = {0.0f, 0.0f};
    capsule.center2 = {0.0f, torsoHeight };
    capsule.radius = halfWidth;

    b2ShapeDef capsuleShapeDef = b2DefaultShapeDef();
    capsuleShapeDef.density = 1.0f;
    capsuleShapeDef.material.friction=  MOVING_FRICTION;

    b2CreateCapsuleShape(bodyId, &capsuleShapeDef, &capsule);

    //TODO: Add sensors


}

void CharacterActor::goUp() {
    if(isOnGround) {
        b2Vec2 velocity = getVelocity();
        velocity.y = -jumpSpeed;
        setVelocity(velocity);
    }
}

void CharacterActor::goLeft() {
    if(isOnGround) {
        b2Vec2 velocity = getVelocity();
        velocity.x = -walkSpeed;
        setVelocity(velocity);
        facingRight = false;
    }

    // For fast motion
    // NOTE: Put here so it also works after landing
    setFriction(MOVING_FRICTION);
}

void CharacterActor::goRight() {
    if(isOnGround) {
        b2Vec2 velocity = getVelocity();
        velocity.x = walkSpeed;
        setVelocity(velocity);
        facingRight = true;
    }

    // For fast motion
    // NOTE: Put here so it also works after landing
    setFriction(MOVING_FRICTION);
}

void CharacterActor::goDown() {
}

void CharacterActor::goNowhere() {
    if(isOnGround) {
        b2Vec2 velocity = getVelocity();
        velocity.x = 0;
        setVelocity(velocity);
    }

    // So we can stand on slopes
    // NOTE: Put here so it also works after landing
    setFriction(STANDING_FRICTION);
}

void CharacterActor::doInteractWith() {
}

void CharacterActor::setFriction(float friction) {
    this->friction = friction;
}
