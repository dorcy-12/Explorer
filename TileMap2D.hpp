#pragma once


#include "raylib-cpp-master/include/raylib-cpp.hpp"
#include "tileson.hpp"
#include <unordered_map>
#include "box2d/box2d.h"

class TileMap2D
{
public:


    /**
     * preload all the resources of the tiled Map
     *
     * @param fileName tiled Map path
     */
    TileMap2D(const std::string& fileName);

    virtual ~TileMap2D();

    void draw(const raylib::Camera2D& camera, const raylib::Color &tint = WHITE);

    /** Update Animations
     *
     * @param elapsedTime in Seconds
     */

    void updateAnimations(float elapsedTime);

    raylib::Vector2 getMapSize() const;

    void generatePhysicsObjects(b2WorldId worldId);

    void drawCollisionShapes(const raylib::Camera2D &camera, const raylib::Color &color);

private:
    /**
     * Draws each layer
     */
    void drawLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint);
    void drawTileLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint);
    void drawImageLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint);


    /**
     * Loads an image into a raylib 2D texture
     * @param baseDir base directory of all images
     * @param fileName Name of the image
     */
    void loadImage(const std::string &baseDir, const std::string &fileName);

    /**
     * Tiled Map
     */
    std::unique_ptr<tson::Map> tileMap;

    /**
     * Hashmap with all the needed textures with their names as keys.
     *  MUST BE FIXED, WAY TOO INEFFICIENT.
     */
    std::unordered_map<std::string, std::shared_ptr<raylib::Texture>> textures;

    unsigned groundLayerIdx;


    /** Calculates a layer's offset.
     *
     * @param layer the layer
     * @param camera the camera viewing the layer
     * @param worldScale the scale factor that converts from world coordinates to display coordinates
     * @param offset a base offset to add to the layer's own offsets
     */
    const raylib::Vector2 calcLayerOffset(
     tson::Layer &layer, const raylib::Camera2D &camera,
     const raylib::Vector2 &offset = raylib::Vector2::Zero()
     );
};


