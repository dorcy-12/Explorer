#pragma once
#include "Command.hpp"

class InputHandler {
public:
  InputHandler();
  virtual ~InputHandler();
  void handleInput(Command& command);
};


