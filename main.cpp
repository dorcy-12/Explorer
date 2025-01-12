
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


int main() {
    // Initialize raylib window
    InitWindow(screenWidth, screenHeight, "raylib [texture] example - sprite anim");

    Tileson t;
    const string fileName = DATADIR "ScarfyMap.tmj";  // Ensure DATADIR is defined correctly
    std::unique_ptr<Map> tileMap = t.parse(fileName); //screenHeight/2/ Use std::unique_ptr
    auto mapSize = tileMap->getSize();
    const string groundLayerName = "Ground";
    int groundLayerIndex = 0;

    // Check if the tile map loaded correctly
    if (tileMap->getStatus() != ParseStatus::OK) {
        throw std::runtime_error(TextFormat("Failed to load tile-map %s. Error: %s",
            fileName.c_str(), tileMap->getStatusMessage().c_str()));
    }

    auto tilesets = tileMap->getTilesets();

    // preload all images
    auto layers = tileMap->getLayers();
    for (const auto& layer : layers) {
        groundLayerIndex++;
        if (groundLayerName == layer.getName()) {
            cout<<"i found the tilelayer" << groundLayerIndex<< endl;

            break;
        }
    }

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(::RAYWHITE);

        EndDrawing();

    }
    CloseWindow();



    // Unload the image textures when the program ends
    for (auto& [key, texture] : imageLayerTextures) {
        UnloadTexture(texture);
    }


    return 0;
}
*/