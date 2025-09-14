//
// Created by Dorcy Agape on 19.10.24.
//

#include "TileMap2DScene.hpp"
#include "TrackingCamera2D.hpp"

const static TrackingCamera2D defaultCamera;

TileMap2DScene::TileMap2DScene() {
}

TileMap2DScene::TileMap2DScene(const std::string &filename):
    tileMap(std::make_shared<TileMap2D>(filename)){
}

TileMap2DScene::~TileMap2DScene() {

}

void TileMap2DScene::draw() {
    ClearBackground(RAYWHITE);

    const auto &camera = getCamera();

    if(camera) {
        camera->BeginMode();
    }

    if(tileMap) {
        auto currCamera = camera ? *camera : defaultCamera;
        tileMap->draw(currCamera, ::RAYWHITE);
    }


    drawActors();

    if (debugFlags) {
        if (debugFlags & DEBUG_SCENE_COLLISION_SHAPES) {
            if (tileMap) {
                auto &currCamera = camera ? *camera : defaultCamera;
                tileMap->drawCollisionShapes(currCamera, RED)
            }
        }
        if ((debugFlags & DEBUG_SCENE_GRID) != 0) {
            if (tileMap) {
                tileMap->drawGrid(currCamera, DARKGRAY);
            }
        }
        drawDebug();
    }

    if(camera) {
        camera->EndMode();
    }
}

shared_ptr<Scene> TileMap2DScene::update() {
    const float elapsedTime = GetFrameTime();
    if(tileMap) {
        tileMap->updateAnimations(elapsedTime);
    }
    return Scene::update(elapsedTime);
}

raylib::Vector2 TileMap2DScene::get2DSize() const {
    if(tileMap) {
        return tileMap->getMapSize();
    }
    return Scene::get2DSize();
}
