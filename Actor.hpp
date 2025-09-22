#pragma once
#include "Command.hpp"
#include "raylib-cpp.hpp"
#include "box2d/box2d.h"


class Actor : public Command {
public:
    Actor();

	virtual ~Actor();

	/** Updates this actor's state.
	 * This could include the animation state, triggering of sound effects, etc.
	 *
	 * IMPORTANT: Physics simulation is handled at the scene level.
	 *
	 * @return bool true if this actor still exists, and false if it has "exited
	 * the scene, never to return" (e., dead)
	 */
	virtual bool update(float elapsedTime, b2WorldId worldId);

	/** Draws this actor to the display.
	 *
	 * NOTE: Exclude additional camera zoom, or your camera zoom effect may not work
	 */
	virtual void draw();

	/** Draw's this actor's bounding box.
	 *
	 * @param color the colour to draw
	 *
	 */
	virtual void drawBoundingBox(const Color &color);

	/** Returns this actor's bounding box in world coordinages (i.e., the smallest
	 * rectangle that contains this actor, including all its sub-components).
	 *
	 * @param worldScale the scale factor from world coordinates to display coordinates
	 * NOTE: Exclude additional camera zoom, or your camera zoom effect may not work
	 */
	virtual raylib::Rectangle getBoundingBox();

	/** Gets the actor's position in the world.
	 */
	[[nodiscard]] b2Vec2 getPosition() const;

	/** Sets the actor's position.
	 */
	void setPosition(const b2Vec2 &position) const;

	/** Gets the actor's velocity.
	 */
	[[nodiscard]] b2Vec2 getVelocity() const;

	/** Sets the actor's velocity.
	 */
	void setVelocity(const b2Vec2 &velocity) const;

	/** Returns true if the actor is moving.
	 */
	[[nodiscard]] bool isMoving() const;

	/** Create this actor's physics body.
	 *
	 * NOTES:
	 * - The base class silently creates nothing. Child-classes should implement this method.
	 * - The physics body pointer should be written to the physicsBody field (e.g.,
	 *   physicsBody = world.CreateBody(&bodyDef);
	 *
	 * @param world_id the physics world id to add the actor to
	 * @param position the actor's initial position in the scene
	 * @param velocity the actor's initial velocity
	 */
	virtual void createPhysicsBody(b2WorldId world_id,
		const b2Vec2 &position, const b2Vec2 &velocity = b2Vec2{0.0f, 0.0f});

	/** Deletes the physics body, thereby removing this actor from a physics world.
	 */
	void deletePhysicsBody();

public:
	/** Is this actor currently on the ground?.
	 * This is set by the physics simulation.
	 */
	bool isOnGround{};

	/** The previous state of isOnGround.
	 * This should be updated at the end of the update() method.
	 */
	bool wasOnGround{};

protected:
	/** The actor's physics body.
	 */
	b2BodyId physicsBodyId{};
};
