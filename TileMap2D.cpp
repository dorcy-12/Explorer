//
// Created by Dorcy Agape on 19.10.24.
//

#include "TileMap2D.hpp"
#include "string"
#include "TileMapLoader.hpp"

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


    switch (layer.getType()) {
        case (LayerType::TileLayer):
            drawTileLayer(layer, camera, newOffset,::RAYWHITE);
        break;
        case (LayerType::ImageLayer):
            drawImageLayer(layer,camera, newOffset, parallaxEffect, ::RAYWHITE);
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

void TileMap2D::drawImageLayer(Layer &layer, const raylib::Camera2D &camera, const raylib::Vector2 &offset,const raylib::Vector2 &parallax,  const raylib::Color &tint) {
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
             screenOriginPosition.y + parallax.y,
            repeatX ? GetScreenWidth() * scale  : texWidth,
            repeatY ? GetScreenHeight() * scale : texHeight);
        raylib::Vector2 origin = raylib::Vector2(0,0);
        texture->DrawTiled(sourceRec, destRec, origin);
    } else {
        texture->Draw(offset, tint);
    }
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
