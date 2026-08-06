#pragma once

#include "Scene.hpp"
#include "tileson.hpp"
#include "TileMap2D.hpp"

using namespace std;

class TileMap2DScene: public Scene{
public:
    TileMap2DScene();

    TileMap2DScene(const std::string &filename);

    virtual ~TileMap2DScene();

    void draw() override;

    shared_ptr<Scene> update(float elapsedTime) override;

    raylib::Vector2 get2DSize() const override;

protected:
    shared_ptr<TileMap2D> tileMap;
};



