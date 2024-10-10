#pragma once
#include <memory>
#include <list>
#include "Command.h"
using namespace std;

class Actor;

class Scene : public Command{
public:
    Scene();
    virtual ~Scene();

    virtual void loadResources();
    virtual void unloadResources();
    virtual void start();
    virtual shared_ptr<Scene> update();

    virtual void draw();
    virtual void goUp();
    virtual void goDown();
    virtual void goLeft();
    virtual void goRight();
    virtual void goNowhere();
    virtual void interact();

protected:
    shared_ptr<Command> playerAvatar;
    list<shared_ptr<Actor>> actors;

    int groundYPos;

    float gravity;

};



