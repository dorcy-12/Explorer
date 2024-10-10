//
// Created by Dorcy Agape on 13.09.24.
//

#include "InputHandler.hpp"
#include "raylib.h"

InputHandler::InputHandler() {}
InputHandler::~InputHandler() {}
void InputHandler::handleInput(Command &command) {
    bool moving = false;

    if (IsKeyDown(KEY_UP)) {
        command.goUp();
        moving = true;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        command.goRight();
        moving = true;
    }else if (IsKeyDown(KEY_LEFT)) {
        command.goLeft();
        moving = true;
    }
    if (!moving) {
        command.goNowhere();
    }
    if (IsKeyDown(KEY_SPACE)) {
        command.interact();
    }

}


