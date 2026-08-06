//
// Created by Dorcy Agape on 25.08.25.
//

#pragma once
#include "Actor.hpp"
#include "box2d/box2d.h"

class CharacterActor : public Actor {
public:
	CharacterActor();
	~CharacterActor() override;

	void createPhysicsBody(b2WorldId world_id,
		 const b2Vec2 &position, const b2Vec2 &velocity) override;

	void goUp() override;

	void goLeft() override;

	void goRight() override;

	void goDown() override;

	void goNowhere() override;

	/** Select, push, pull, punch, fire, etc.
	 */
	virtual void doInteractWith();

    /**
     *
     * @param elapsedTime
     * @param worldId
     * @return
     */
    bool update(float elapsedTime, b2WorldId worldId) override;


protected:
    /**
     *
     * @param worldId
     * @return
     */
    bool senseGround(b2WorldId worldId);
	/**
	 * @param friction
	 * sets The character's friction with objects around it like the ground
	 */
	void setFriction(float friction);

	/**
	 * indicates the direction the character is facing. (left or right)
	 */
	bool facingRight;

	// Parameters that control character's motion
	float jumpSpeed;
	float walkSpeed;

	// Physics shape parameters
	float widthScale;

	/** The "are we on the ground" sensor.
	 */
	//PhysicsObject groundSensor;
	//int groundContactCount;

	/** Need to tweak friction depending on the situation.
	 */
	//PhysicsObject frictionAdjuster;

	float friction;

    int groundContactCount = 0;

    b2ShapeId physicsBodyShapeId;

    b2ShapeId physicsSensorShapeId;

};