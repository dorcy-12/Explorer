#pragma once
#include "TileMap2DScene.hpp"


class TileMapLoader {
public:
    TileMapLoader();
    virtual ~TileMapLoader();
    static std::unique_ptr<tson::Map> loadTileMap(const std::string &filename);
};

