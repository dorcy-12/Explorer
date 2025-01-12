//
// Created by Dorcy Agape on 13.09.24.
//

#include "InputHandler.hpp"
#include "raylib.h"

InputHandler::InputHandler() {}
InputHandler::~InputHandler() {}
void InputHandler::handleInput(Command &listener) {
    bool moving = false;

    if (IsKeyDown(KEY_UP)) {
        listener.goUp();
        moving = true;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        listener.goRight();
        moving = true;
    }else if (IsKeyDown(KEY_LEFT)) {
        listener.goLeft();
        moving = true;
    }
    if (!moving) {
        listener.goNowhere();
    }
    if (IsKeyDown(KEY_SPACE)) {
        listener.interact();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        listener.doExit();
    }

    int key;
    while((key = GetKeyPressed()) != 0) {
        listener.handleKeyPress(key);
    }


}


