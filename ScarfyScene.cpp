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
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    tileMap = std::make_shared<TileMap2D>(DATADIR "ScarfyMap.tmj");

    tileMap->generatePhysicsObjects(worldId); // we generate the ground layer physics.

    auto mapSize = get2DSize();

    raylib::Vector2 sceneSize(mapSize.x, mapSize.y);
    raylib::Vector2 cameraOffset(screenWidth/2, screenHeight/2);
    raylib::Vector2 cameraTarget(screenWidth/2,  (550+sceneSize.y ) - (screenHeight/2) );

    // todo - Fix the Ground Position
    groundYPos = cameraTarget.y + screenHeight + 370;

    auto camera = make_shared<TrackingCamera2D>(cameraOffset, cameraTarget, 0, 0.7f);

    setCamera(camera);

    auto scarfy = make_shared<Scarfy>(); // create our scarfy actor

    scarfy->position = raylib::Vector2(screenWidth / 2, groundYPos);

    this->camera->setTarget(scarfy);
    playerAvatar = scarfy;
    actors.emplace_back(scarfy);
}
