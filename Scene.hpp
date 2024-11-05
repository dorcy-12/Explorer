#pragma once
#include <memory>
#include <list>
#include "Command.hpp"
#include "TrackingCamera2D.hpp"
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

    /** Returns True if the Window should Close*/
    virtual bool shouldQuit();

    /** Sets the scene's camera
     */
    void setCamera(std::shared_ptr<TrackingCamera2D> &camera);

    /** Gets the scene's camera.
     */
    std::shared_ptr<TrackingCamera2D>& getCamera();

    /** Get's the scene's 2D size.
     * Defaults to the screen size if child-classes don't override it
     */
    virtual raylib::Vector2 get2DSize() const;

    virtual void draw();
    void goUp()     override;
    void goDown()   override;
    void goLeft()   override;
    void goRight()  override;
    void goNowhere()override;
    void interact() override;
    void doExit()   override;

protected:

    void drawActors();

    shared_ptr<Command> playerAvatar;
    list<shared_ptr<Actor>> actors;

    /** The camera.
         */
    std::shared_ptr<TrackingCamera2D> camera;

    int groundYPos;

    float gravity;

    bool exit;

};



