#pragma once
#include "Actor.hpp"
#include "raylib.h"
#include "Camera2D.hpp"

/*
 *Tweaked version of the raylib 2Dcamera to follow a target
 */

class TrackingCamera2D: public raylib::Camera2D, public Actor{
public:

    TrackingCamera2D(const TrackingCamera2D &camera);

    TrackingCamera2D();

    TrackingCamera2D(Vector2 position, Vector2 target, float rotation = 0.0f, float zoom = 1.0f);

    void setTarget(const std::shared_ptr<Actor> &target);

    std::shared_ptr<Actor> getTarget();

    bool update (bool onGround) override;

private:
    std::shared_ptr<Actor> targetActor;
    raylib::Vector2 targetOffset;

};
