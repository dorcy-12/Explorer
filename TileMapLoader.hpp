#pragma once
#include "TileMap2DScene.hpp"


class TileMapLoader {
public:
    TileMapLoader();
    ~TileMapLoader();
    std::unique_ptr<tson::Map> load2DMap(std::string &filename);
};

