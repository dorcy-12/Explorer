//
// Created by Dorcy Agape on 25.08.25.
//

#include "CharacterActor.hpp"


#define MOVING_FRICTION 0.2f
#define STANDING_FRICTION 1.0f

CharacterActor::CharacterActor() {
    facingRight = true;
    setFriction(MOVING_FRICTION);

    walkSpeed = 12;
    jumpSpeed = 2.2f * walkSpeed;
}

CharacterActor::~CharacterActor() {
}

void CharacterActor::createPhysicsBody(b2WorldId world_id,  const b2Vec2 &position, const b2Vec2 &velocity) {
    raylib::Rectangle boundingBox = getBoundingBox();

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.linearVelocity = velocity;
    float halfWidth = boundingBox.width / 2.0f;

    //TODO: Friction Callback

    b2BodyId bodyId = b2CreateBody(world_id, &bodyDef);

    float torsoHeight = boundingBox.height - halfWidth; //?
    float torsoHalfHeight = torsoHeight / 2.0f;

    b2Capsule capsule;
    capsule.center1 = {0.0f, -torsoHalfHeight};
    capsule.center2 = {0.0f, torsoHalfHeight };
    capsule.radius = halfWidth / 2.0f;

    b2ShapeDef capsuleShapeDef = b2DefaultShapeDef();
    capsuleShapeDef.density = 1.0f;
    capsuleShapeDef.material.friction=  MOVING_FRICTION;
    capsuleShapeDef.material.customColor = b2_colorChocolate;

    b2CreateCapsuleShape(bodyId, &capsuleShapeDef, &capsule);

    physicsBodyId = bodyId;

    //TODO: Add ground sensors


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
