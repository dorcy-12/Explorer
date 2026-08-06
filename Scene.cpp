//
// Created by Dorcy Agape on 08.10.24.
//

#include "Scene.hpp"

#include <utility>
#include "raylib-cpp.hpp"
#include "Actor.hpp"
#include "MainMenuScene.hpp"
#include "cmake-build-debug/_deps/box2d-src/src/joint.h"

#define kPixelsPerMeter 50.0f
#define DEFAULT_WORLDSCALE 50.0f


Scene::Scene(){
   //Set up the worldId
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y =  9.81f * kPixelsPerMeter;

    const b2WorldId worldId = b2CreateWorld(&worldDef);
    this->physicsWorldId = worldId;

    exit = false;

    timeStep = 1.0f/60.0f;

    subStepCount = 4;

    // Set up the scale factor of this world
    // All length values are returned using this ratio
    // by passing in pixels per meter. All length values return are in pixels.
    b2SetLengthUnitsPerMeter(kPixelsPerMeter);

    // set the debug
    debugFlags = 0;
}
Scene::~Scene() {
    Scene::unloadResources();
}

void Scene::loadResources() {
}

void Scene::unloadResources() {
    playerAvatar.reset();
    actors.clear();
}
void Scene::setCamera(const std::shared_ptr<TrackingCamera2D> &camera) {
    this->camera = camera;
}

std::shared_ptr<TrackingCamera2D>& Scene::getCamera() {
    return camera;
}

void Scene::start() {
}

shared_ptr<Scene> Scene::update(const float elapsedTime) {
    b2World_Step(physicsWorldId, timeStep, subStepCount);
    for(const auto &actor : actors) {
        actor->update(elapsedTime, physicsWorldId);
    }

    if(camera) {
        camera->update(elapsedTime, physicsWorldId);
    }

    if(exit) {
        exit = false;
        return std::make_shared<MainMenuScene>();
    }
    return nullptr;
}

void Scene::addActor(const std::shared_ptr<Actor>& actor, const b2Vec2 &position, const b2Vec2 &velocity, bool addOnTop) {
    if(!actor) {
        return;
    }

    actor->createPhysicsBody(physicsWorldId, position, velocity);

    if(addOnTop) {
        actors.emplace_back(actor);
    } else {
        actors.emplace_front(actor);
    }
}

void Scene::setPlayerAvatar(std::shared_ptr<Command> playerAvatar) {
    this->playerAvatar = std::move(playerAvatar);
}


void Scene::clearActors() {
    for (const auto&actor:actors) {
        actor->deletePhysicsBody();
    }
    actors.clear();
}


void Scene::draw() {
    if(camera) {
        camera->BeginMode();
    }

    ClearBackground(RAYWHITE);

    drawActors();

    if(debugFlags) {
        drawDebug();
    }

    if(camera) {
        camera->EndMode();
    }
}

void Scene::drawDebug() {
    if ((debugFlags & (DEBUG_PLAYER_BBOX | DEBUG_ACTOR_BBOXES)) != 0) {
        for(auto &actor:actors) {
            if(actor == playerAvatar &&(debugFlags & DEBUG_PLAYER_BBOX) != 0) actor->drawBoundingBox(RED);
            else if ((debugFlags & DEBUG_ACTOR_BBOXES) !=0) actor->drawBoundingBox(ORANGE);
        }
    }
    else if ((debugFlags & DEBUG_SCENE_PHYSICS)!=0) {
        b2DebugDraw* draw = drawPhysics.GetDebugDraw();
        auto scale = 1.0f/camera->zoom;

        // Get the actual camera bounds
        Vector2 topLeft = camera ->GetScreenToWorld({0, 0});
        Vector2 bottomRight = camera -> GetScreenToWorld( {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});

        // Add some margin so objects near edges are visible
        constexpr float margin = 50.0f; // Adjust as needed

        draw->drawShapes = true;
        draw->drawingBounds={
            .lowerBound = {topLeft.x - margin, topLeft.y - margin},
            .upperBound = {bottomRight.x + margin, bottomRight.y + margin}
        };

        // Draw the shapes
        b2World_Draw(physicsWorldId, draw);
    }
}




b2WorldId Scene::getPhysicsWorld() const {
    return physicsWorldId;
}

raylib::Vector2 Scene::get2DSize() const {
    return {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
}

void Scene::drawActors() const {
    if(!actors.empty()) {
        for(auto &actor: actors) {
            actor->draw();
        }
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
void Scene::doExit() {
    exit = true;
};

void Scene::handleKeyPress(int key) {
    switch(key) {
        case '1':
            debugFlags = debugFlags ^ DEBUG_PLAYER_BBOX;
            break;
        case '2':
            debugFlags = debugFlags ^ DEBUG_ACTOR_BBOXES;
        break;
        case '3':
            debugFlags = debugFlags ^ DEBUG_SCENE_GRID;
        break;
        case '4':
            debugFlags = debugFlags ^ DEBUG_SCENE_COLLISION_SHAPES;
        break;
        case '5':
            debugFlags = debugFlags ^ DEBUG_SCENE_PHYSICS;
        break;
        default:
            ;
    }
}

