#pragma once
#include "Actor.hpp"
#include "raylib.h"
#include "Camera2D.hpp"

/*
 *Tweaked version of the raylib 2Dcamera to follow a target
 */

class TrackingCamera2D: public raylib::Camera2D, public Actor{
public:

    TrackingCamera2D();

    /** Create a tracking 2D camera with the given parameters.
     *
     * @param offset offset to the camera's origin in pixels (this is usually the screen centre)
     * @param target the initial camera target (i.e., where the camera is looking) in world coordinates
     * @param rotation the camera's rotation (anti-clockwise) in radians
     * @param zoom a zoom factor from world coordinates to screen coordinates (should normally be
     * the scene's worldScale, unless you want to add additional zoom)
     */
    TrackingCamera2D(::Vector2 offset, ::Vector2 target,
            float rotation = 0.0f, float zoom = 1.0f);

    TrackingCamera2D(const TrackingCamera2D &camera) = delete; // disables copying of the camera object

    void setTarget(const std::shared_ptr<Actor> &target);

    std::shared_ptr<Actor> getTarget();

    bool update (float elapsedTime, b2WorldId worldId) override ;

private:
    /** Performs basic camera initialization.
     */
    void init();

    std::shared_ptr<Actor> targetActor;
    raylib::Vector2 targetOffset;

    /** The camera's position in the scene.
     */
    raylib::Vector2 position;

    /** The camera's velocity.
     */
    raylib::Vector2 velocity;

};
