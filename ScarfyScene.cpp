//
// Created by Dorcy Agape on 09.10.24.
//

#include "ScarfyScene.hpp"
#include "TrackingCamera2D.hpp"
#include "Common.hpp"
#include "TileMap2D.hpp"

using namespace std;

ScarfyScene::ScarfyScene() {
}

ScarfyScene::~ScarfyScene() {
}

void ScarfyScene::loadResources(){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    tileMap = std::make_shared<TileMap2D>(DATADIR "ScarfyMap.tmj");
    const b2WorldId worldId = getPhysicsWorld();


    const auto mapSize = get2DSize();

    const raylib::Vector2 sceneSize(mapSize.x, mapSize.y);
    raylib::Vector2 cameraOffset(static_cast<float>(screenWidth) /2.0f, static_cast<float>(screenHeight)/2.0f);
    raylib::Vector2 cameraTarget(static_cast<float>(screenWidth)/2.0f,  (450+sceneSize.y ) - (static_cast<float>(screenHeight)/2.0f) );

    const auto camera = make_shared<TrackingCamera2D>(cameraOffset, cameraTarget, 0, 0.7f);
    setCamera(camera);

    tileMap->generatePhysicsObjects(worldId, *camera); // we generate the ground layer physics.

    // todo - Fix the Ground Position
    const float groundYPos = cameraTarget.y + (static_cast<float>(screenHeight)/4.0f);

    const auto scarfy = make_shared<Scarfy>(); // create our scarfy actor
    const b2Vec2 initialPos = {static_cast<float>(screenWidth) / (2.0f * 0.7f), groundYPos};
    addActor(scarfy, initialPos);
    setPlayerAvatar(scarfy);


    camera->setTarget(scarfy);
}
