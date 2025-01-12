//
// Created by Dorcy Agape on 19.10.24.
//

#include "TileMap2D.hpp"
#include "string"
#include "TileMapLoader.hpp"
#include "box2d/box2d.h"
#include "cmake-build-debug/_deps/box2d-src/src/world.h"

using namespace std;
using namespace tson;

static raylib::Rectangle toRayLibRect(tson::Rect rect) {
    return {
        static_cast<float>(rect.x),
        static_cast<float>(rect.y),
        static_cast<float>(rect.width),
        static_cast<float>(rect.height)
    };
}

TileMap2D::TileMap2D(const std::string &fileName) {
    tileMap = TileMapLoader::loadTileMap(fileName);

    // Check if the tile map loaded correctly
    if (tileMap->getStatus() != ParseStatus::OK) {
        throw std::runtime_error(TextFormat("Failed to load tile-map %s. Error: %s",
            fileName.c_str(), tileMap->getStatusMessage().c_str()));
    }

    std::string baseDir = GetDirectoryPath(fileName.c_str());

    auto tilesets = tileMap->getTilesets();

    // preload all images
    auto layers = tileMap->getLayers();
    for (const auto& layer : layers) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            loadImage(baseDir, layer.getImage());
        }
    }

    // preload all tiles
    for (auto &tileset: tilesets) {
        auto &imgPath = tileset.getImage();
        if(!imgPath.empty()) {
            cout << imgPath << endl;
        }else {
            for(const auto& tile : tileset.getTiles()) {
                auto &image = tile.getImage();
                if(!image.empty()) {
                    loadImage(baseDir, image.string());
                }
            }
        }
    }
    groundLayerIdx = 0;
    string groundLayerName = "Ground";
    for (const auto& layer : layers) {
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
    auto layers = tileMap->getLayers();

    for( auto &layer : layers) {
        drawLayer(layer, camera, raylib::Vector2(0,0), tint);
    }

}

void TileMap2D::updateAnimations(float elapsedTime) {
    // ##### FIXME! ###### To-do
}

raylib::Vector2 TileMap2D::getMapSize() const {
    Vector2i size = tileMap->getSize();
    return {static_cast<float>(size.x), static_cast<float>(size.y)};
}

void TileMap2D::drawLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint) {

    raylib::Vector2 newOffset = calcLayerOffset(layer, camera, offset);
    switch (layer.getType()) {
        case (LayerType::TileLayer):
            drawTileLayer(layer, camera, newOffset,::RAYWHITE);
        break;
        case (LayerType::ImageLayer):
            drawImageLayer(layer,camera, newOffset,::RAYWHITE);
        break;
        case LayerType::Group:
        {
            for(auto &subLayer : layer.getLayers()) {
                drawLayer(subLayer, camera, newOffset, tint);
            }
            break;
        }
        default:
            // Fail
            auto *errMsg = TextFormat("ERROR: Unsupported Tileson layer type %u",
                    layer.getType());
            throw std::runtime_error(errMsg);
    }
}

void TileMap2D::drawTileLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint) {
    for (auto& [pos, tileObject] : layer.getTileObjects()) {
        tson::Tile* tile = tileObject.getTile();
        tson::Tileset *tileset = tile->getTileset();
        auto tileImage = tileset->getImage();
        auto imagePath = tileImage.empty() ? tile->getImage() : tileImage;
        auto texIter = textures.find(imagePath.string());
        if(texIter == textures.end()) {
            auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n",
                imagePath.string().c_str());
            throw std::runtime_error(errMsg);
        }
        auto &texture = texIter->second;

        raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());
        auto tilePos = tileObject.getPosition();

        // NB: Tiled draws an image from the bottom left while Raylib draws from its top left
        // The images then seem to be lower in raylib that is why - drawRect.height is needed
        raylib::Vector2 drawPos(tilePos.x + offset.x, (tilePos.y + offset.y) - drawRect.height);

        texture->Draw(drawRect, drawPos, tint);

    }
}

void TileMap2D::drawImageLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset, const raylib::Color &tint) {
    auto imageName = layer.getImage();

    auto texIter = textures.find(imageName);
    if(texIter ==  textures.end()) {
        auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n",
            imageName.c_str());
        throw std::runtime_error(errMsg);
    }
    auto &texture = texIter->second;

    bool repeatX = layer.hasRepeatX();
    bool repeatY = layer.hasRepeatY();

    if(repeatX || repeatY) {
        auto screenOriginPosition = camera.GetScreenToWorld(raylib::Vector2(0.0f, 0.0f));
        auto texWidth = texture->GetWidth();
        auto texHeight = texture->GetHeight();
        float scale = 1.0f / camera.GetZoom(); // makes sure the image layers are scaled horizontally accordingly
        raylib::Rectangle sourceRec(
            repeatX ? offset.x : 0 ,
            repeatY ? offset.y: 0,
            texWidth, texHeight);
        raylib::Rectangle destRec(
             screenOriginPosition.x,
             screenOriginPosition.y,
            repeatX ? GetScreenWidth() * scale  : texWidth,
            repeatY ? GetScreenHeight() * scale : texHeight);
        raylib::Vector2 origin = raylib::Vector2(0,0);
        texture->DrawTiled(sourceRec, destRec, origin);
    } else {
        texture->Draw(offset, tint);
    }
}

void TileMap2D::generatePhysicsObjects(b2WorldId worldId) {
    std::vector<b2Vec2> vertices;
    auto tileLayers = tileMap->getLayers();

    if(groundLayerIdx < tileLayers.size()) {

        auto &layer = tileLayers[groundLayerIdx];
        auto layerOffsetTson = layer.getOffset();
        b2Vec2 layerOffset(layerOffsetTson.x, layerOffsetTson.y);

        if(layer.getType() == LayerType::TileLayer) {
            for (auto& [pos, tileObject] : layer.getTileObjects()) {
                tson::Tile* tile = tileObject.getTile();
                auto tilePos = tileObject.getPosition();
                b2Vec2 tileOffset = b2Vec2(tilePos.x , tilePos.y) + layerOffset;

                auto tileObjects = tile->getObjectgroup();
                auto collisionShapes = tileObjects.getObjects();
                for (auto& collisionShape : collisionShapes) {
                    b2BodyDef bodyDef = b2DefaultBodyDef();
                    bodyDef.type = b2_staticBody;
                    auto shape = collisionShape.getPosition();
                    auto shapePos = b2Vec2(static_cast<float>(shape.x), static_cast<float>(shape.y) ) + tileOffset;
                    bodyDef.position = {shapePos.x, shapePos.y};
                    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

                    switch(collisionShape.getObjectType()) {
                        case ObjectType::Polygon:
                        case ObjectType::Polyline: {
                            vertices.clear();
                            auto &polyVertices = collisionShape.getPolygons();
                            if (polyVertices.size() >= 3) {
                                for (auto &point: polyVertices) {
                                    b2Vec2 p = {static_cast<float>(point.x), static_cast<float>(point.y)};
                                    vertices.push_back(p);
                                }
                                b2Hull polyHull = b2ComputeHull(vertices.data(),static_cast<int32_t>(polyVertices.size()));
                                b2Polygon polygon= b2MakePolygon(&polyHull, 0);

                                b2ShapeDef polyShapeDef = b2DefaultShapeDef();
                                b2CreatePolygonShape(bodyId,&polyShapeDef, &polygon);
                            }
                            break;
                        }
                        case ObjectType::Rectangle: {
                            auto &rectSize = collisionShape.getSize();
                            b2Polygon rectangle = b2MakeBox((float)rectSize.x/2.0f, rectSize.y/2.0f);

                            b2ShapeDef rectShapeDef = b2DefaultShapeDef();
                            rectShapeDef.density=1.0f;
                            rectShapeDef.friction= 0.3f;

                            b2CreatePolygonShape(bodyId,&rectShapeDef, &rectangle);

                            break;
                        }
                        default:
                            TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u", collisionShape.getObjectType());
                    }
                }
            }
        }
    }
}

void TileMap2D::drawCollisionShapes(const raylib::Camera2D &camera, const raylib::Color &color) {
    std::vector<b2Vec2> vertices;
    auto tileLayers = tileMap->getLayers();

    if(groundLayerIdx < tileLayers.size()) {

        auto &layer = tileLayers[groundLayerIdx];
        raylib::Vector2 newOffset = calcLayerOffset(layer, camera);

        if(layer.getType() == LayerType::TileLayer) {
            for (auto& [pos, tileObject] : layer.getTileObjects()) {
                tson::Tile* tile = tileObject.getTile();
                auto tilePos = tileObject.getPosition();
                raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());

                raylib::Vector2 tileOffset = raylib::Vector2(tilePos.x, tilePos.y) + newOffset - raylib::Vector2(0.0f, drawRect.height);

                auto tileObjects = tile->getObjectgroup();
                auto collisionShapes = tileObjects.getObjects();

                for (auto& collisionShape : collisionShapes) {
                    const auto shape = collisionShape.getPosition();
                    const raylib::Vector2 shapeOffset = raylib::Vector2(static_cast<float>(shape.x), static_cast<float>(shape.y)) + tileOffset;


                    switch(collisionShape.getObjectType()) {
                        case ObjectType::Polygon:
                        case ObjectType::Polyline: {
                            vertices.clear();
                            auto &polyVertices = collisionShape.getPolygons();
                            if (polyVertices.size() > 1) {
                                auto &lastPos = polyVertices[polyVertices.size() - 1];
                                raylib::Vector2 prevPos = raylib::Vector2(static_cast<float>(lastPos.x), static_cast<float>(lastPos.y))  + shapeOffset;
                                for(auto &currPoint : polyVertices) {
                                    raylib::Vector2 currPos = raylib::Vector2(static_cast<float>(currPoint.x), static_cast<float>(currPoint.y)) + shapeOffset;
                                    DrawLineEx(prevPos, currPos, 1, color);
                                    prevPos = currPos;
                                }
                            }

                            break;
                        }
                        case ObjectType::Rectangle: {
                            auto &rectSize = collisionShape.getSize();
                            raylib::Rectangle rect(shapeOffset.x, shapeOffset.y,
                                static_cast<float>(rectSize.x), static_cast<float>(rectSize.y));
                            DrawRectangleLinesEx(rect,4,color);
                            break;
                        }
                        default:
                            TraceLog(LOG_ERROR, "Error: Unrecognized collision shape object type: %u", collisionShape.getObjectType());
                    }
                }
            }
        }
    }
}

const raylib::Vector2 TileMap2D::calcLayerOffset(tson::Layer &layer, const raylib::Camera2D &camera,
    const raylib::Vector2 &offset) {
    //convert Parallax Components from tson Vector2i to Raylib Vector2
    auto pFactorTson = layer.getParallax();
    auto pOriginTson = layer.getMap()->getParallaxOrigin();
    raylib::Vector2 parallaxFactor(pFactorTson.x, pFactorTson.y);
    raylib::Vector2 parallaxOrigin(pOriginTson.x, pOriginTson.y);

    //convert from layerOffset from tson Vector2i to Raylib Vector2
    auto layerOffsetTson = layer.getOffset();
    raylib::Vector2 layerOffset(layerOffsetTson.x, layerOffsetTson.y);

    auto cameraOffset = camera.GetWorldToScreen(parallaxOrigin) + raylib::Vector2(-GetScreenWidth() / 2, -GetScreenHeight() / 2);
    raylib::Vector2 parallaxEffect = cameraOffset *  parallaxFactor;
    raylib::Vector2 newOffset = layerOffset - parallaxEffect;

    return newOffset;
}

void TileMap2D::loadImage(const std::string &baseDir, const std::string &fileName) {
    if(textures.contains(fileName)) {
        // Already loaded
        return;
    }

    std::string fullPath = (!baseDir.empty()) ?
        TextFormat("%s/%s", baseDir.c_str(), fileName.c_str()) :
        fileName;

    textures[fileName] = std::make_shared<raylib::Texture>(fullPath);
}
