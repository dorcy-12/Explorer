//
// Created by Dorcy Agape on 19.10.24.
//

#include "TrackingCamera2D.hpp"

TrackingCamera2D::TrackingCamera2D(const TrackingCamera2D &camera):
        raylib::Camera2D(camera) {
}

TrackingCamera2D::TrackingCamera2D() {
}

TrackingCamera2D::TrackingCamera2D(Vector2 position, Vector2 target, float rotation, float zoom) {
}

void TrackingCamera2D::setTarget(std::shared_ptr<Actor> &target) {
    this->target = target;
}

std::shared_ptr<Actor> TrackingCamera2D::getTarget() {
    return target;
}

bool TrackingCamera2D::update(bool onGround) {
    return Actor::update(onGround);
}
