//
// Created by Dorcy Agape on 09.10.24.
//

#include "ScarfyScene.hpp"

using namespace std;

ScarfyScene::ScarfyScene() {
}

ScarfyScene::~ScarfyScene() {
}

void ScarfyScene::loadResources(){
    shared_ptr<Scarfy> scarfy= make_shared<Scarfy>(); // create our scarfy actor

    int screenWidth = GetScreenWidth();
    scarfy->position = Vector2{static_cast<float>(screenWidth) / 2, 0.0f};

    playerAvatar = scarfy;
    actors.emplace_back(scarfy);
}
