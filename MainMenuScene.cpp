
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
   DrawText("Scarfy!", 200, 100, 120, RED); // Title

   //Play and Quit Buttons

   startClicked = GuiButton((Rectangle){340,300,120,24},"Play");
   quitClicked = GuiButton((Rectangle){340,340,120,24},GuiIconText(ICON_EXIT,"Quit"));
   if ( quitClicked) shouldQuit();
}

shared_ptr<Scene> MainMenuScene::update() {
   if (startClicked) return make_shared<ScarfyScene>();
   return nullptr;
}

bool MainMenuScene::shouldQuit() {
   return quitClicked;
}
