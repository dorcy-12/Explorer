//
// Created by Dorcy Agape on 19.10.24.
//

#include "TileMap2D.hpp"
#include "TileMapLoader.hpp"
#include "box2d/box2d.h"
#include "string"
// #include "cmake-build-debug/_deps/box2d-src/src/world.h"

using namespace std;
using namespace tson;

static raylib::Rectangle toRayLibRect(const tson::Rect rect) {
    return {static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(rect.width),
            static_cast<float>(rect.height)};
}

TileMap2D::TileMap2D(const std::string &fileName) {
    tileMap = TileMapLoader::loadTileMap(fileName);

    // Check if the tile map loaded correctly
    if (tileMap->getStatus() != ParseStatus::OK) {
        throw std::runtime_error(TextFormat("Failed to load tile-map %s. Error: %s", fileName.c_str(),
                                            tileMap->getStatusMessage().c_str()));
    }

    std::string baseDir = GetDirectoryPath(fileName.c_str());

    auto tilesets = tileMap->getTilesets();

    // preload all images
    auto layers = tileMap->getLayers();
    for (const auto &layer: layers) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            loadImage(baseDir, layer.getImage(), layer.hasRepeatX());
        }
    }

    // preload all tiles
    for (auto &tileset: tilesets) {
        auto &imgPath = tileset.getImage();
        if (!imgPath.empty()) {
            cout << imgPath << endl;
        } else {
            for (const auto &tile: tileset.getTiles()) {
                auto &image = tile.getImage();
                if (!image.empty()) {
                    loadImage(baseDir, image.string(), false);
                }
            }
        }
    }

    // TODO: Check if we can combine the upper for loop and this lower one. Basically doing the same thing.

    groundLayerIdx = 0;
    string groundLayerName = "Ground";
    for (const auto &layer: layers) {
        if (groundLayerName.compare(layer.getName()) == 0) {
            break;
        }
        groundLayerIdx++;
    }
    if (groundLayerIdx >= layers.size()) {
        throw std::runtime_error(TextFormat("Failed to find Ground Layer"));
    }
}

TileMap2D::~TileMap2D() = default;

void TileMap2D::draw(const raylib::Camera2D &camera, const raylib::Color &tint) {
    for (auto &layers = tileMap->getLayers(); auto &layer: layers) {
        drawLayer(layer, camera, raylib::Vector2(0, 0), tint);
    }
}

// TODO: Check if this makes sense (Why are we doing this?)
void TileMap2D::drawGrid(const raylib::Camera2D &camera, const raylib::Color &color) {
    if (auto &layers = tileMap->getLayers(); groundLayerIdx < layers.size()) {
        auto &layer = layers[groundLayerIdx];
        const raylib::Vector2 offset = calcLayerOffset(layer, camera);
        const auto mapSize = layer.getSize();
        const auto tileSize = tileMap->getTileSize();
        constexpr float lineThickness = 2.0f;
        for (int y = 0; y < mapSize.y; y++) {
            const float ypos = offset.y + static_cast<float>(y * tileSize.y);
            const raylib::Vector2 startPos = {offset.x, ypos};
            const raylib::Vector2 endPos = {offset.x + static_cast<float>(mapSize.x * tileSize.x), ypos};
            DrawLineEx(startPos, endPos, lineThickness, color);
        }
        for (int x = 0; x < mapSize.x; x++) {
            const float xpos = offset.x + static_cast<float>(x * tileSize.x);
            const raylib::Vector2 startPos = {xpos, offset.y};
            const raylib::Vector2 endPos = {xpos, offset.y + static_cast<float>(mapSize.y * tileSize.y)};
            DrawLineEx(startPos, endPos, lineThickness, color);
        }
    }
}

void TileMap2D::drawCollisionShapes(const raylib::Camera2D &camera, const raylib::Color &color) {
    std::vector<b2Vec2> vertices;
    auto tileLayers = tileMap->getLayers();
    const float lineThickness = 2.0f;
    if (groundLayerIdx < tileLayers.size()) {

        auto &layer = tileLayers[groundLayerIdx];
        const raylib::Vector2 newOffset = calcLayerOffset(layer, camera);


        for (auto &[pos, tileObject]:
             layer.getTileObjects()) { // pos is tile position in grid, e.g (3,5) -> row 3, column 5
            tson::Tile *tile = tileObject.getTile();
            auto tilePos = tileObject.getPosition(); // pos here is world pixel coordinates.
            raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());

            raylib::Vector2 tileOffset =
                    raylib::Vector2(tilePos.x, tilePos.y) + newOffset - raylib::Vector2(0.0f, drawRect.height);

            auto &tileObjects = tile->getObjectgroup();
            auto collisionShapes = tileObjects.getObjects();

            for (auto &collisionShape: collisionShapes) {
                const auto shape = collisionShape.getPosition();
                const raylib::Vector2 shapeOffset = raylib::Vector2(static_cast<float>(shape.x), static_cast<float>(shape.y)) + tileOffset;

                switch (collisionShape.getObjectType()) {
                    case ObjectType::Polygon:
                    case ObjectType::Polyline: {
                        vertices.clear();
                        auto &polyVertices = collisionShape.getPolygons();
                        if (polyVertices.size() > 1) {
                            auto &lastPos = polyVertices[polyVertices.size() - 1];
                            raylib::Vector2 prevPos =
                                    raylib::Vector2(static_cast<float>(lastPos.x), static_cast<float>(lastPos.y)) +
                                    shapeOffset;
                            for (auto &currPoint: polyVertices) {
                                raylib::Vector2 currPos = raylib::Vector2(static_cast<float>(currPoint.x),
                                                                          static_cast<float>(currPoint.y)) +
                                                          shapeOffset;
                                DrawLineEx(prevPos, currPos, lineThickness, color);
                                prevPos = currPos;
                            }
                        }

                        break;
                    }
                    case ObjectType::Rectangle: {
                        auto &rectSize = collisionShape.getSize();
                        raylib::Rectangle rect(shapeOffset.x, shapeOffset.y, static_cast<float>(rectSize.x),
                                               static_cast<float>(rectSize.y));
                        DrawRectangleLinesEx(rect, 4, color);
                        break;
                    }
                    default: TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u", collisionShape.getObjectType());
                }
                DrawCircle(shapeOffset.x,shapeOffset.y, 20.0f, ::BLACK);
                //TraceLog(LOG_INFO, "Tile position in world: (%d,%d)",tilePos.x,tilePos.y);
            }
        }
    }
}

void TileMap2D::updateAnimations(float elapsedTime) {
    // ##### FIXME! ###### To-do
}

void TileMap2D::generatePhysicsObjects(b2WorldId worldId, const raylib::Camera2D &camera) {
    std::vector<b2Vec2> vertices;
    auto tileLayers = tileMap->getLayers();
    auto tileSizeTson = tileMap->getTileSize();
    b2Vec2 tileSize_2{tileSizeTson.x / (2.0f), tileSizeTson.y / (2.0f)};
    int bodyCount = 0;
    if (groundLayerIdx < tileLayers.size()) {

        auto &layer = tileLayers[groundLayerIdx];
        //auto layerOffsetTson = layer.getOffset();
        raylib::Vector2 layerOffsetRl = calcLayerOffset(layer, camera);
        b2Vec2 layerOffset = {layerOffsetRl.x, layerOffsetRl.y};


        if (layer.getType() == LayerType::TileLayer) {
            for (auto &[pos, tileObject]: layer.getTileObjects()) {
                ::Tile *tile = tileObject.getTile();
                auto tilePos = tileObject.getPosition();
                raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());

                b2Vec2 tileOffset = b2Vec2{tilePos.x, tilePos.y} + layerOffset - b2Vec2{0.0f, drawRect.height};
                //TraceLog(LOG_INFO, "Tile position in world: (%f,%f)",tilePos.x,tilePos.y);
                auto &tileObjects = tile->getObjectgroup();
                auto collisionShapes = tileObjects.getObjects();
                for (auto &collisionShape: collisionShapes) {

                    b2BodyDef bodyDef = b2DefaultBodyDef();
                    bodyDef.type = b2_staticBody;

                    auto shape = collisionShape.getPosition();
                    auto shapePos = b2Vec2{static_cast<float>(shape.x), static_cast<float>(shape.y)} + tileOffset;


                    switch (collisionShape.getObjectType()) {
                        case ObjectType::Polygon:
                        case ObjectType::Polyline: {
                            auto &polySize = collisionShape.getSize();
                            b2Vec2 halfSize = {static_cast<float>(polySize.x) / 2.0f, static_cast<float>(polySize.y) / 2.0f};

                            bodyDef.position = shapePos + halfSize;
                            b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
                            vertices.clear();
                            auto &polyVertices = collisionShape.getPolygons(); //
                            if (polyVertices.size() >= 3) {
                                for (auto &point: polyVertices) {
                                    auto vertice = b2Vec2{static_cast<float>(point.x), static_cast<float>(point.y)};
                                    b2Vec2 p = vertice - halfSize;
                                    vertices.push_back(p);
                                }
                                b2Hull polyHull = b2ComputeHull(vertices.data(), static_cast<int32_t>(polyVertices.size()));
                                //printf("Valid Hull!\n");
                                if (polyHull.count >= 3) {
                                    b2Polygon polygon = b2MakePolygon(&polyHull, 0);

                                    b2ShapeDef polyShapeDef = b2DefaultShapeDef();
                                    polyShapeDef.density = 1.0f;
                                    polyShapeDef.material.friction = 0.0f;
                                    polyShapeDef.enableSensorEvents = true;
                                    b2CreatePolygonShape(bodyId, &polyShapeDef, &polygon);
                                    bodyCount++;
                                } else {
                                    TraceLog(LOG_WARNING, "Invalid hull for polygon at (%f,%f)", shapePos.x, shapePos.y);
                                }
                            }
                            break;
                        }
                        case ObjectType::Rectangle: {
                            auto &rectSize = collisionShape.getSize();

                            b2Vec2 halfSize = {static_cast<float>(rectSize.x) / 2.0f, static_cast<float>(rectSize.y) / 2.0f};
                            bodyDef.position = shapePos + halfSize;
                            b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

                            b2Polygon rectangle = b2MakeBox(static_cast<float>(rectSize.x) / 2.0f, static_cast<float>(rectSize.y) / 2.0f);
                            b2ShapeDef rectShapeDef = b2DefaultShapeDef();
                            rectShapeDef.density = 1.0f;
                            rectShapeDef.material.friction = 0.0f;
                            rectShapeDef.enableSensorEvents = true;
                            b2CreatePolygonShape(bodyId, &rectShapeDef, &rectangle);
                            bodyCount++;
                            break;
                        }
                        default:
                            TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u",
                                     collisionShape.getObjectType());
                    }
                }
            }
        }
    }
    TraceLog(LOG_INFO, "Created %d physics bodies", bodyCount);
}


raylib::Vector2 TileMap2D::getMapSize() const {
    Vector2i size = tileMap->getSize();
    return {static_cast<float>(size.x), static_cast<float>(size.y)};
}

void TileMap2D::drawLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                          const raylib::Color &tint) {

    //raylib::Vector2 newOffset = calcLayerOffset(layer, camera, offset);
    raylib::Vector2 newOffset = {layer.getOffset().x, layer.getOffset().y};
    switch (layer.getType()) {
        case (LayerType::TileLayer):
            drawTileLayer(layer, camera, newOffset, ::RAYWHITE);
            break;
        case (LayerType::ImageLayer):
            drawImageLayer(layer, camera, newOffset, ::RAYWHITE);
            break;
        case LayerType::Group: {
            for (auto &subLayer: layer.getLayers()) {
                drawLayer(subLayer, camera, newOffset, tint);
            }
            break;
        }
        default:
            // Fail
            auto *errMsg = TextFormat("ERROR: Unsupported Tileson layer type %u", layer.getType());
            throw std::runtime_error(errMsg);
    }
}

void TileMap2D::drawTileLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                              const raylib::Color &tint) {
    for (auto &[pos, tileObject]: layer.getTileObjects()) {
        tson::Tile *tile = tileObject.getTile();
        tson::Tileset *tileset = tile->getTileset();
        auto tileImage = tileset->getImage();
        auto imagePath = tileImage.empty() ? tile->getImage() : tileImage;
        auto texIter = textures.find(imagePath.string());
        if (texIter == textures.end()) {
            auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n", imagePath.string().c_str());
            throw std::runtime_error(errMsg);
        }
        auto &texture = texIter->second;

        raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());
        auto tilePos = tileObject.getPosition();

        // NB: Tiled draws an image from the bottom left while Raylib draws from its top left
        // The images then seem to be lower in raylib that is why - drawRect.height is needed.
        raylib::Vector2 drawPos(tilePos.x + offset.x, (tilePos.y + offset.y) - drawRect.height);

        texture->Draw(drawRect, drawPos, tint);
    }
}

void TileMap2D::drawImageLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,
                               const raylib::Color &tint) {
    const auto& imageName = layer.getImage();

    auto texIter = textures.find(imageName);
    if (texIter == textures.end()) {
        auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n", imageName.c_str());
        throw std::runtime_error(errMsg);
    }
    auto &texture = texIter->second;

    bool repeatX = layer.hasRepeatX();
    bool repeatY = layer.hasRepeatY();


    if (repeatX || repeatY) {
        auto screenOriginPosition = camera.GetScreenToWorld(raylib::Vector2(0.0f, 0.0f));
        auto texWidth = texture->GetWidth();
        auto texHeight = texture->GetHeight();
        float scale = 1.0f / camera.GetZoom(); // makes sure the image layers are scaled horizontally accordingly
        raylib::Rectangle sourceRec(
            repeatX ? offset.x : 0,
            repeatY ? offset.y : 0,
            repeatX ? GetScreenWidth() * scale : texWidth,
            repeatY ? GetScreenHeight()* scale : texHeight);
        raylib::Rectangle destRec(screenOriginPosition.x, screenOriginPosition.y,
                                  repeatX ? GetScreenWidth() * scale : texWidth,
                                  repeatY ? GetScreenHeight()* scale : texHeight);
        raylib::Vector2 origin = raylib::Vector2(0, 0);
        texture->Draw(sourceRec, screenOriginPosition); // raylib::DrawTexturePro()

    } else {
        texture->Draw(offset, tint);
    }
}



const raylib::Vector2 TileMap2D::calcLayerOffset(tson::Layer &layer, const raylib::Camera2D &camera,
                                                 const raylib::Vector2 &offset) {
    // convert Parallax Components from tson Vector2i to Raylib Vector2
    auto pFactorTson = layer.getParallax();
    auto pOriginTson = layer.getMap()->getParallaxOrigin();
    raylib::Vector2 parallaxFactor(pFactorTson.x, pFactorTson.y);
    raylib::Vector2 parallaxOrigin(pOriginTson.x, pOriginTson.y);

    // convert from layerOffset from tson Vector2i to Raylib Vector2
    auto layerOffsetTson = layer.getOffset();
    raylib::Vector2 layerOffset(layerOffsetTson.x, layerOffsetTson.y);

    auto cameraOffset =
            camera.GetWorldToScreen(parallaxOrigin) + raylib::Vector2(-GetScreenWidth() / 2, -GetScreenHeight() / 2);
    raylib::Vector2 parallaxEffect = cameraOffset * (raylib::Vector2(1.0f, 1.0f) - parallaxFactor);
    raylib::Vector2 newOffset = offset + layerOffset - parallaxEffect;

    return newOffset;
}

void TileMap2D::loadImage(const std::string &baseDir, const std::string &fileName, bool repeat) {
    if (textures.contains(fileName)) {
        // Already loaded
        return;
    }

    std::string fullPath = (!baseDir.empty()) ? TextFormat("%s/%s", baseDir.c_str(), fileName.c_str()) : fileName;
    auto texture = std::make_shared<raylib::Texture>(fullPath);
    if (repeat) SetTextureWrap(*texture, TEXTURE_WRAP_REPEAT);
    textures[fileName] = texture;
}
