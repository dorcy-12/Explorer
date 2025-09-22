
#include "MainMenuScene.hpp"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "ScarfyScene.hpp"


MainMenuScene::MainMenuScene() {
   quitClicked= false;
   startClicked = false;
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::draw() {
   //Title
   int screenWidth = GetScreenWidth();
   int screenHeight = GetScreenHeight();
   int buttonWidth = 120;
   int buttonHeight = 24;
   DrawText("Scarfy!", screenWidth / 3, screenHeight /3, 120, RED); // Title

   //Play and Quit Buttons

   startClicked = GuiButton((Rectangle){
      static_cast<float>((screenWidth / 2) - (buttonWidth / 2)),
      static_cast<float>((screenHeight *3)/4),
      120.0f,
      24.0f
   }, "Play");

   quitClicked = GuiButton((Rectangle){
      static_cast<float>((screenWidth / 2) - (buttonWidth / 2)),
      static_cast<float>(((screenHeight *3)/4) + 40),
      120,
      24
   },GuiIconText(ICON_EXIT,"Quit"));
   if ( quitClicked) shouldQuit();
}

shared_ptr<Scene> MainMenuScene::update(float elapsedTime) {
   if (startClicked) return make_shared<ScarfyScene>();
   return nullptr;
}

bool MainMenuScene::shouldQuit() {
   return quitClicked;
}
