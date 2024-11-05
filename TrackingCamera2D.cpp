//
// Created by Dorcy Agape on 19.10.24.
//

#include "TrackingCamera2D.hpp"


TrackingCamera2D::TrackingCamera2D(const TrackingCamera2D &camera):
        raylib::Camera2D(camera) {
    this->targetActor = targetActor;
}

TrackingCamera2D::TrackingCamera2D() {
}

TrackingCamera2D::TrackingCamera2D(Vector2 offset, Vector2 target, float rotation, float zoom):
                    raylib::Camera2D(offset, target, rotation, zoom) {
    targetOffset = ::Vector2{0.0f, 0.0f};

}

void TrackingCamera2D::setTarget(const std::shared_ptr<Actor> &targetActor) {
    this->targetActor = targetActor;
}

std::shared_ptr<Actor> TrackingCamera2D::getTarget() {
    return targetActor;
}

bool TrackingCamera2D::update(bool onGround) {
    if (!targetActor) return true;
    if(targetActor->velocity.x > 0) {
        int screenWidth = GetScreenWidth();
        targetOffset = raylib::Vector2(screenWidth / 4.0f, 0.0f);
    } else if(targetActor->velocity.x < 0) {
        int screenWidth = GetScreenWidth();
        targetOffset = -raylib::Vector2(screenWidth / 4.0f, 0.0f);
    }
    this->target = Vector2Lerp(this->target, targetActor->position + targetOffset , 0.1f);

    return Actor::update(onGround);
}
