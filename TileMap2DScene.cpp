//
// Created by Dorcy Agape on 19.10.24.
//

#include "TileMap2DScene.hpp"
#include "TrackingCamera2D.hpp"

const static TrackingCamera2D defaultCamera;

TileMap2DScene::TileMap2DScene() {
}

TileMap2DScene::TileMap2DScene(const std::string &fileName):
    tileMap(std::make_shared<TileMap2D>(fileName)){
}

TileMap2DScene::~TileMap2DScene() {

}

void TileMap2DScene::draw() {

    if(camera) {
        camera->BeginMode();
    }

    if(tileMap) {
        auto currCamera = camera ? *camera : defaultCamera;
        tileMap->draw(currCamera, ::RAYWHITE);
    }


    drawActors();

    if(camera) {
        camera->EndMode();
    }
}

shared_ptr<Scene> TileMap2DScene::update() {
    float elapsedTime = GetFrameTime();
    if(tileMap) {
        tileMap->updateAnimations(elapsedTime);
    }

    // ##### FIXME! ###### Handle collisions?

    return Scene::update();
}

raylib::Vector2 TileMap2DScene::get2DSize() const {
    if(tileMap) {
        return tileMap->getMapSize();
    } else {
        return Scene::get2DSize();
    }
}
