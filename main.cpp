
#include <raylib.h>
#include "InputHandler.hpp"
#include "ScarfyScene.hpp"
#include "MainMenuScene.hpp"
#include <stddef.h>
#include <stdlib.h>


using namespace std;

const int screenWidth = 1200;
const int screenHeight = 600;

void showErrorAndExit(const char *errMsg) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(errMsg, 20, 20, 20, RED);
        EndDrawing();
    }

    exit(EXIT_FAILURE);
}


int main()
{
    int retVal = EXIT_SUCCESS;


    InitWindow(screenWidth, screenHeight, "raylib [texture] example - sprite anim");
    InitAudioDevice();  // Initialize audio device and context
    SetMasterVolume(0.4f);  // Set master volume (listener)
    try {

        InputHandler input_handler;

        shared_ptr<Scene> currScene = make_shared<MainMenuScene>();
        currScene -> loadResources();
        currScene -> start();

        bool quit = false;

        SetTargetFPS(60);

        while (!quit)
        {
            input_handler.handleInput(*currScene);
            auto nextScene = currScene -> update();

            if (nextScene) {
                nextScene -> loadResources();
                nextScene -> start();
                currScene = nextScene;
            }
            quit = WindowShouldClose() || currScene->shouldQuit();

            BeginDrawing();
            ClearBackground(RAYWHITE);
            currScene -> draw();
            EndDrawing();
        }

        CloseAudioDevice();

        CloseWindow();

    }catch(runtime_error& e) {
        showErrorAndExit(e.what());
        retVal = EXIT_FAILURE;
    }


    return 0;
}


/*

#include <iostream>
#include <memory>    // Include for std::unique_ptr
#include <string>    // Include for std::string
#include <unordered_map>
#include <vector>    // Include for std::vector
#include "tileson.hpp" // Include your Tileson header
#include "Common.hpp"
#include "raylib-cpp-master/include/raylib-cpp.hpp"

using namespace std;
using namespace  tson;

const int screenWidth = 1200;
const int screenHeight = 600;
int Size = 0;
unordered_map<std::string, raylib::Texture> imageLayerTextures;
vector<tson::Layer*> imageLayers;

static raylib::Rectangle toRayLibRect(tson::Rect rect) {
    return raylib::Rectangle(rect.x, rect.y, rect.width, rect.height);
}
void DrawImageLayer(tson::Layer& layer, const raylib::Camera2D &camera,raylib::Vector2 offset, raylib::Vector2 parallaxEffect, ::Color tint) {
    auto imageName = layer.getImage();

    auto texIter = imageLayerTextures.find(imageName);
    if(texIter == imageLayerTextures.end()) {
        auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n",
            imageName.c_str());
        throw std::runtime_error(errMsg);
    }
    auto &texture = texIter->second;

    bool repeatX = layer.hasRepeatX();
    bool repeatY = layer.hasRepeatY();

     if(repeatX || repeatY) {
        auto screenOriginPosition = camera.GetScreenToWorld(raylib::Vector2(0.0f, 0.0f));
        auto texWidth = texture.GetWidth();
        auto texHeight = texture.GetHeight();
        float scale = 1.0f / camera.GetZoom(); // makes sure the image layers are scaled horizontally accordingly
        raylib::Rectangle sourceRec(
            repeatX ? offset.x : 0 ,
            repeatY ? offset.y: 0,
            texWidth, texHeight);
        raylib::Rectangle destRec(
             screenOriginPosition.x,
             screenOriginPosition.y + parallaxEffect.y,
            repeatX ? GetScreenWidth() * scale  : texWidth,
            repeatY ? GetScreenHeight() * scale : texHeight);
        raylib::Vector2 origin = raylib::Vector2(0,0);
        texture.DrawTiled(sourceRec, destRec, origin);
    } else {
        texture.Draw(offset, tint);
        cout<< "we are not repeating"<<endl;
    }
}
void DrawTileLayer(tson::Layer& layer, const raylib::Camera2D &camera, raylib::Vector2 offset, ::Color tint) {
    for (auto& [pos, tileObject] : layer.getTileObjects()) {
        tson::Tile* tile = tileObject.getTile();
        tson::Tileset *tileset = tile->getTileset();
        auto tileImage = tileset->getImage();
        auto imagePath = tileImage.empty() ? tile->getImage() : tileImage;
        auto texIter = imageLayerTextures.find(imagePath.string());
        if(texIter == imageLayerTextures.end()) {
            auto *errMsg = TextFormat("ERROR: Tile-map is missing image: %s\n",
                imagePath.string().c_str());
            throw std::runtime_error(errMsg);
        }
        auto &texture = texIter->second;

        raylib::Rectangle drawRect = toRayLibRect(tileObject.getDrawingRect());
        auto tilePos = tileObject.getPosition();
        raylib::Vector2 drawPos(tilePos.x + offset.x, (tilePos.y + offset.y) - drawRect.height); // Tiled draws froom top left while Raylib draws from bottom left.
        texture.Draw(drawRect, drawPos, tint);
    }
}

void draw(unique_ptr<Map> &tileMap, const raylib::Camera2D &camera,::Color tint) {
    auto layers = tileMap->getLayers();
    auto screenOriginPosition = camera.GetScreenToWorld(raylib::Vector2(0.0f, 0.0f));
    for (auto &layer: layers) {
        auto pFactorTson = layer.getParallax();
        auto pOriginTson = layer.getMap()->getParallaxOrigin();
        raylib::Vector2 parallaxFactor(pFactorTson.x, pFactorTson.y);
        raylib::Vector2 parallaxOrigin(pOriginTson.x, pOriginTson.y);
        auto layerOffset = layer.getOffset();
        raylib::Vector2 offset(layerOffset.x, layerOffset.y);
        auto cameraOffset = camera.GetWorldToScreen(parallaxOrigin) + raylib::Vector2(-GetScreenWidth() / 2, -GetScreenHeight() / 2);
        raylib::Vector2 parallaxEffect = cameraOffset *  parallaxFactor;
        raylib::Vector2 newOffset = offset - parallaxEffect;



        if ( layer.getType() == LayerType::TileLayer ) {
            DrawTileLayer(layer, camera, newOffset,::RAYWHITE);
        }else if (layer.getType() == LayerType::ImageLayer) {
            DrawImageLayer(layer,camera, newOffset, parallaxEffect, ::RAYWHITE);
        }
    }
    DrawCircle(screenOriginPosition.x,screenOriginPosition.y,10,::RED);
}

int main() {
    // Initialize raylib window
    InitWindow(screenWidth, screenHeight, "TMJ Map Example");

    Tileson t;
    const string fileName = DATADIR "ScarfyMap.tmj";  // Ensure DATADIR is defined correctly
    std::unique_ptr<Map> tileMap = t.parse(fileName); //screenHeight/2/ Use std::unique_ptr
    auto mapSize = tileMap->getSize();
    raylib::Vector2 sceneSize(mapSize.x, mapSize.y);
    raylib::Vector2 cameraOffset(screenWidth/2, screenHeight);
    raylib::Vector2 cameraTarget(screenWidth/2,  (550+sceneSize.y ) - (screenHeight/2) );
    auto camera = make_shared<raylib::Camera2D>(cameraOffset, cameraTarget, 0, 0.7f);


    // Check if the tile map loaded correctly
    if (tileMap->getStatus() != ParseStatus::OK) {
        throw std::runtime_error(TextFormat("Failed to load tile-map %s. Error: %s",
            fileName.c_str(), tileMap->getStatusMessage().c_str()));
    }

    auto tilesets = tileMap->getTilesets();

    // preload all images
    auto layers = tileMap->getLayers();
    for (const auto& layer : layers) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            auto &image = layer.getImage();
            string fullPath = DATADIR + image;
            imageLayerTextures[image] = raylib::Texture(fullPath);
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
                string fullPath = DATADIR + image.string();
                if(!image.empty()) {
                    imageLayerTextures[image] = raylib::Texture(fullPath);
                }
            }
        }
    }


    while (!WindowShouldClose()) {
        // Start drawing
        BeginDrawing();
        ClearBackground(::RAYWHITE);  // Clear before drawing

        if (IsKeyDown(KEY_UP)) {
            camera->target.y -= 4;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            camera->target.y += 4;
        }
        if (IsKeyDown(KEY_LEFT)) {
            camera->target.x -= 4;
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            camera->target.x += 4;
        }


        if(camera) {
            camera->BeginMode();
        }

        draw(tileMap, *camera, ::RAYWHITE);

        if(camera) {
            camera->EndMode();
        }


        EndDrawing();
    }


    // Unload the image textures when the program ends
    for (auto& [key, texture] : imageLayerTextures) {
        UnloadTexture(texture);
    }
    CloseWindow();

    return 0;
}
*/