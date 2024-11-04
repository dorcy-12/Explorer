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

}

void TrackingCamera2D::setTarget(const std::shared_ptr<Actor> &targetActor) {
    this->targetActor = targetActor;
}

std::shared_ptr<Actor> TrackingCamera2D::getTarget() {
    return targetActor;
}

bool TrackingCamera2D::update(bool onGround) {
    // ## IS THIS NECESSARY? ###
    this->target.x += targetActor->velocity.x;
    //this->target.y += targetActor->velocity.y;
    return onGround;
}
