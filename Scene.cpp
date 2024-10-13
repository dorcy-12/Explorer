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
        actor->position.x += actor->velocity.x;
        actor->position.y += actor->velocity.y;

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
    }
    return nullptr;
}

void Scene::draw() {
    ClearBackground(RAYWHITE);
    DrawLine(0,groundYPos,GetScreenWidth(),groundYPos,BLACK);
    if (actors.size() > 0) {
        for (auto &actor: actors) {
            actor->draw();
        }
    }else {
        DrawText("This Scene has no Content Available", 20, 20, 30, RED);
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

