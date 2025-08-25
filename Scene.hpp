#pragma once
#include <memory>
#include <list>
#include "Command.hpp"
#include "TrackingCamera2D.hpp"
#include "box2d/box2d.h"
#include "cmake-build-debug/_deps/box2d-src/include/box2d/id.h"

using namespace std;

class Actor;


// Show the player avatar's bounding box
#define DEBUG_PLAYER_BBOX (1 << 0)

// Show the other actor's bouunding boxes
#define DEBUG_ACTOR_BBOXES (1 << 1)

// Show the scene's grid (e.g., tile-map grid)
#define DEBUG_SCENE_GRID (1 << 2)

// Show the scene's static collision shapes
#define DEBUG_SCENE_COLLISION_SHAPES (1 << 3)

// Show the physics debug info
#define DEBUG_SCENE_PHYSICS (1 << 4)

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
    void handleKeyPress(int key) override;

protected:

    void drawActors();

    shared_ptr<Command> playerAvatar;
    list<shared_ptr<Actor>> actors;

    /** The camera.
         */
    std::shared_ptr<TrackingCamera2D> camera;

    /** Flags for debugging (can draw debug info to screen or print to the log).
     */
    unsigned debugFlags;

    int groundYPos;

    float gravity;

    bool exit;

    b2WorldId worldId;
};



