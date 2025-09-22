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
    bodyDef.motionLocks.angularZ = true;
    float halfWidth = boundingBox.width / 2.0f;

    //TODO: Friction Callback

    b2BodyId bodyId = b2CreateBody(world_id, &bodyDef);

    float torsoHeight = boundingBox.height - halfWidth; //?
    float torsoHalfHeight = torsoHeight / 2.0f;

    b2Capsule capsule;
    capsule.center1 = {0.0f, -torsoHalfHeight};
    capsule.center2 = {0.0f, torsoHalfHeight };
    capsule.radius = halfWidth/2;

    b2ShapeDef capsuleShapeDef = b2DefaultShapeDef();
    capsuleShapeDef.density = 1.0f;
    capsuleShapeDef.material.friction =  MOVING_FRICTION;
    capsuleShapeDef.material.customColor = b2_colorChocolate;

    physicsBodyShapeId = b2CreateCapsuleShape(bodyId, &capsuleShapeDef, &capsule);

    physicsBodyId = bodyId;

    float footSensorOffset = 3.0f;
    b2ShapeDef footSensorShapeDef = b2DefaultShapeDef();
    footSensorShapeDef.isSensor = true;
    footSensorShapeDef.enableSensorEvents = true;
    b2Circle footSensor;
    footSensor.radius = halfWidth/2;
    footSensor.center = {0.0f, torsoHalfHeight + footSensorOffset };
    physicsSensorShapeId = b2CreateCircleShape(bodyId, &footSensorShapeDef, &footSensor);
}

void CharacterActor::goUp() {
    if(isOnGround) {
        b2Vec2 velocity = getVelocity();
        velocity.y = -jumpSpeed;
        setVelocity(velocity);
    }
    b2Shape_SetFriction(physicsBodyShapeId, MOVING_FRICTION);
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
    b2Shape_SetFriction(physicsBodyShapeId, MOVING_FRICTION);
}

void CharacterActor::goRight() {
    if(isOnGround) {

        b2Vec2 velocity = getVelocity();
        velocity.x = walkSpeed;
        setVelocity(velocity);

        b2Vec2 actualVel = b2Body_GetLinearVelocity(physicsBodyId);



        facingRight = true;
    }

    // For fast motion
    // NOTE: Put here so it also works after landing
    b2Shape_SetFriction(physicsBodyShapeId, MOVING_FRICTION);
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
    b2Shape_SetFriction(physicsBodyShapeId, STANDING_FRICTION);
}

void CharacterActor::doInteractWith() {
}

void CharacterActor::setFriction(float friction) { this->friction = friction; }

bool CharacterActor::update(float elapsedTime, b2WorldId worldId) {
    bool glound  = senseGround(worldId);

    return glound;
}

bool CharacterActor::senseGround(b2WorldId worldId) {


    b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);

    // Only loop if there are actually events
    if (sensorEvents.beginCount == 0 && sensorEvents.endCount == 0) {
        //TraceLog(LOG_INFO, "Ground no events");
        return true;
    }

    // Process only relevant events
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        b2SensorBeginTouchEvent *event = sensorEvents.beginEvents + i;

        // Quick ID comparison - this is very fast
        if (B2_ID_EQUALS(event->sensorShapeId, physicsSensorShapeId)) {
            groundContactCount++;
            isOnGround = true;
            TraceLog(LOG_INFO, "Ground contact START, count: %d", groundContactCount);
        }
    }

    // Process end events
    for (int i = 0; i < sensorEvents.endCount; ++i) {
        b2SensorEndTouchEvent* endEvent = sensorEvents.endEvents + i;

        // Check if this involves our ground sensor
        if (B2_ID_EQUALS(endEvent->sensorShapeId, physicsSensorShapeId)) {
            groundContactCount--;
            if (groundContactCount <= 0) {
                groundContactCount = 0;  // Clamp to 0
                isOnGround = false;
                TraceLog(LOG_INFO, "Ground contact END, count: %d", groundContactCount);
            }
        }
    }
    return true;
}
