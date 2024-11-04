//
// Created by Dorcy Agape on 08.10.24.
//

#include "Scene.hpp"
#include "raylib-cpp-master/include/raylib-cpp.hpp"
#include "Actor.hpp"

Scene::Scene() {
    int sceneHeight = GetScreenHeight();
    groundYPos = (3 * sceneHeight) / 4;
    gravity = 1;
}

Scene::~Scene() {
}

void Scene::loadResources() {
}

void Scene::unloadResources() {
    playerAvatar.reset();
    actors.clear();
}

void Scene::start() {
}

shared_ptr<Scene> Scene::update() {
    for (auto &actor: actors) {
        actor->position += actor->velocity;

        auto boundingRect = actor->getBoundingBox();

        auto distToGround = groundYPos - (boundingRect.y + boundingRect.height);

        bool isOnGround = distToGround <= 0;

        if (isOnGround) {
            actor->velocity.y = 0;
            actor->position.y += distToGround;
        }else {
            actor->velocity.y += gravity;
        }
        actor->update(isOnGround);
        camera->update(isOnGround);
    }
    return nullptr;
}

void Scene::draw() {
    if(camera) {
        camera->BeginMode();
    }

    ClearBackground(RAYWHITE);

    drawActors();

    if(camera) {
        camera->EndMode();
    }
}


void Scene::goUp() {
   if (playerAvatar) {
       playerAvatar->goUp();
   }
}

void Scene::goDown() {
    if (playerAvatar) {
        playerAvatar->goDown();
    }
}

void Scene::goLeft() {
    if (playerAvatar) playerAvatar->goLeft();
}

void Scene::goRight() {
   if (playerAvatar) playerAvatar->goRight();
}

void Scene::goNowhere() {
    if (playerAvatar) playerAvatar->goNowhere();
}

void Scene::interact() {
    if (playerAvatar) playerAvatar->interact();
}
bool Scene::shouldQuit() {
    return false;
}

void Scene::setCamera(std::shared_ptr<TrackingCamera2D> &camera) {
    this->camera = camera;
}

std::shared_ptr<TrackingCamera2D> & Scene::getCamera() {
    return camera;
}

raylib::Vector2 Scene::get2DSize() const {
    return raylib::Vector2((float)GetScreenWidth(), (float)GetScreenHeight());
}

void Scene::drawActors() {
    if(actors.size()) {
        for(auto &actor: actors) {
            actor->draw();
        }
    }
}