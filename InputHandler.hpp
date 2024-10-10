#pragma once
#include "Command.h"

class InputHandler {
public:
  InputHandler();
  virtual ~InputHandler();
  void handleInput(Command& command);
};


