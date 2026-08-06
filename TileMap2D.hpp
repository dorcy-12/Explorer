#pragma once


#include <unordered_map>
#include "box2d/box2d.h"
#include "raylib-cpp.hpp"
#include "tileson.hpp"

class TileMap2D {
public:
    /**
     * preload all the resources of the tiled Map
     *
     * @param fileName tiled Map path
     */
    explicit TileMap2D(const std::string &fileName);

    virtual ~TileMap2D();

    void draw(const raylib::Camera2D &camera, const raylib::Color &tint = WHITE);

    /** Draws the ground layer's grid.
     */
    void drawGrid(const raylib::Camera2D &camera, const raylib::Color &color = DARKGRAY);

    /**
     * Draws the ground layer's collision shapes.
     */
    void drawCollisionShapes(const raylib::Camera2D &camera, const raylib::Color &color);

    /** Update Animations
     *
     * @param elapsedTime in Seconds
     */

    void updateAnimations(float elapsedTime);

    /** The tile map's size in pixels.
     */
    [[nodiscard]] raylib::Vector2 getMapSize() const;

    /**
     * Generates the Box2d physics bodies of the Scarfy's world.
     *
     * @param worldId
     * @param camera camera, to align drawings and Physical Bodies coordinates
     */
    void generatePhysicsObjects(b2WorldId worldId, const raylib::Camera2D &camera);


private:
    /**
     * Draws each layer
     */
    void drawLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                   const raylib::Color &tint);
    void drawTileLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                       const raylib::Color &tint);
    void drawImageLayer(tson::Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                        const raylib::Color &tint);

    /**
     * Loads an image into a raylib 2D texture
     * @param baseDir base directory of all images
     * @param fileName Name of the image
     * @param repeat if the Image repeats in tiled.
     */
    void loadImage(const std::string &baseDir, const std::string &fileName, bool repeat);

    /**
     * Tiled Map
     */
    std::unique_ptr<tson::Map> tileMap;

    /**
     * Hashmap with all the needed textures with their names as keys.
     * MUST BE FIXED, WAY TOO INEFFICIENT.
     */
    std::unordered_map<std::string, std::shared_ptr<raylib::Texture>> textures;

    /**
     * index of the groundLayer in the tileMap
     */
    unsigned groundLayerIdx;


    /** Calculates a layer's offset.
     *
     * @param layer the layer
     * @param camera the camera viewing the layer
     * @param offset a base offset to add to the layer's own offsets
     */
    const raylib::Vector2 calcLayerOffset(tson::Layer &layer, const raylib::Camera2D &camera,
                                          const raylib::Vector2 &offset = raylib::Vector2::Zero());
};
