//
// Created by Dorcy Agape on 19.10.24.
//

#include "TrackingCamera2D.hpp"

static inline raylib::Vector2 calcAbsTargetOffset(const int screenWidth, const float zoom) {
    return {static_cast<float>(screenWidth) / (6.0f * zoom), 0.0f};
}

TrackingCamera2D::TrackingCamera2D() {
    init();
}

TrackingCamera2D::TrackingCamera2D(Vector2 offset, Vector2 target, float rotation, float zoom) :
    raylib::Camera2D(offset, target, rotation, zoom) {
    position = target;

    init();
}

void TrackingCamera2D::init() {
    const int screenWidth = GetScreenWidth();
    targetOffset = -calcAbsTargetOffset(screenWidth, zoom);
}


void TrackingCamera2D::setTarget(const std::shared_ptr<Actor> &targetActor) {
    this->targetActor = targetActor;
}

std::shared_ptr<Actor> TrackingCamera2D::getTarget() {
    return targetActor;
}

bool TrackingCamera2D::update(float elapsedTime, b2WorldId worldId) {
    if (!targetActor) return true;

    float zoom = GetZoom();
    const auto &[x, y] = targetActor->getVelocity();
    const raylib::Vector2 targetVelocity = {x, y};

    const auto &[i, j] = targetActor->getPosition();
    const raylib::Vector2 targetPosition = {i, j};

    const int screenWidth = GetScreenWidth();

    if (targetVelocity.x > 0) {
        targetOffset = calcAbsTargetOffset(screenWidth, zoom);
    } else if(targetVelocity.x < 0) {
        targetOffset = -calcAbsTargetOffset(screenWidth, zoom);
    }
    // Vector2Lerp is linear interpolation mechanism to make the movements of the camera less harsh
    // Vector2Lerp(start, end, factor); it moves from the start to the end by doing camera_pos = start +(end−start)*factor
    // otherwise changing camera target (as we are doing above) would happen instantaneously (too harsh)
    this->target = Vector2Lerp(this->target, targetPosition + targetOffset , 0.9f);

    return true;
}
