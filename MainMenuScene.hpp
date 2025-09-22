#pragma once
#include "Scene.hpp"


class MainMenuScene: public Scene {

public:
    MainMenuScene();
    ~MainMenuScene() override;
    void draw() override;
    bool shouldQuit() override;
    shared_ptr<Scene> update(float elapsedTime) override;

private:
    bool quitClicked;
    bool startClicked;

};

