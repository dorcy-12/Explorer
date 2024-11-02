#pragma once


#include "raylib-cpp-master/include/raylib-cpp.hpp"

class TileMap2D{
public:
    TileMap2D(std::string& fileName);

    virtual ~TileMap2D();

    virtual void draw(const raylib::Camera2D& camera, const raylib::Color &tint = WHITE);

    /** Update Animations
     *
     * @param elapsedTime in Seconds
     */

    void updateAnimations(float elapsedTime);

    raylib::Vector2 getMapSize() const;

private:
    void drawTile();
    void drawTileLayer();
    void drawImageLayer();

};


