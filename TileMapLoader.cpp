//
// Created by Dorcy Agape on 19.10.24.
//


#define POCKETLZMA_LZMA_C_DEFINE //Must be defined once in a source (.cpp)-file before
#include "pocketlzma.hpp" //Must be declared BEFORE tileson.hpp

#include "tileson.hpp"

#include "TileMapLoader.hpp"

TileMapLoader::TileMapLoader() {
}

TileMapLoader::~TileMapLoader() {
}

std::unique_ptr<tson::Map> TileMapLoader::loadTileMap(const std::string &filename) {
    tson::Tileson tileson;

    if (filename.ends_with('.lzma')) {
        return tileson.parse(filename, std::make_unique<tson::Lzma>());
    }
    return tileson.parse(filename);
}

