#include <raylib.h>
#include "InputHandler.hpp"
#include "ScarfyScene.hpp"
#include <stddef.h>
#include <stdlib.h>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 450;

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

        shared_ptr<Scene> currScene = make_shared<ScarfyScene>();
        currScene -> loadResources();
        currScene -> start();


        SetTargetFPS(60);

        while (!WindowShouldClose())
        {
            input_handler.handleInput(*currScene);
            auto nextScene = currScene -> update();

            if (nextScene) {
                nextScene -> loadResources();
                nextScene -> start();
                currScene = nextScene;
            }

            BeginDrawing();

            currScene -> draw();

            ClearBackground(RAYWHITE);

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