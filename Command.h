#pragma once

class Command {
public:
    Command();
    virtual ~Command();
    virtual void goUp();
    virtual void goDown();
    virtual void goLeft();
    virtual void goRight();
    virtual void goNowhere();
    virtual void interact();
};

