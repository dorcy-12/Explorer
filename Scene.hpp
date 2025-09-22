#pragma once
#include <memory>
#include <list>
#include "Command.hpp"
#include "TrackingCamera2D.hpp"
#include "box2d/box2d.h"
#include "cmake-build-debug/_deps/box2d-src/include/box2d/id.h"
#include "raylib-cpp.hpp"
#include "DrawPhysics.hpp"

using namespace std;

class Actor;


// Show the player avatar's bounding box
#define DEBUG_PLAYER_BBOX (1 << 0)

// Show the other actor's bounding boxes
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
    virtual shared_ptr<Scene> update(float elapsedTime);

    /** Returns True if the Window should Close*/
    virtual bool shouldQuit();

    /** Sets the scene's camera
     */
    void setCamera(const std::shared_ptr<TrackingCamera2D> &camera);

    /** Gets the scene's camera.
     */
    std::shared_ptr<TrackingCamera2D>& getCamera();

    /** Get's the scene's 2D size.
     * Defaults to the screen size if child-classes don't override it
     */
    [[nodiscard]] virtual raylib::Vector2 get2DSize() const;

    /** Draws the debug overlay.
     */
    virtual void drawDebug();

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
    /** Adds an actor to the list, and also adds its physics object.
     *
     * @param actor the actor to add
     * @param position the actor's initial position in the scene
     * @param velocity the actor's initial velocity
     * @param addOnTop set to true to add to the end of the list (so it's drawn on top), and
     * false to add it to the start of the list (so it's drawn behind everything else)
     */
    void addActor(const std::shared_ptr<Actor>& actor,
        const b2Vec2 &position, const b2Vec2 &velocity = b2Vec2{0.0f, 0.0f}, bool addOnTop = true);

    /** Clears the list of actors.
     */
    void clearActors();

    /** Set's the player avatar.
     * Pass nullptr to remove a player without replacing.
     */
    void setPlayerAvatar(std::shared_ptr<Command> playerAvatar);

    /** Draws the scene's actors.
     * The actors are drawn in order (i.e., from first to last, with the last appearing on top).
     */
    void drawActors() const;

    /** Get the physics worldId.
     * This allows scenes to add their own bodies to the physics world.
     */
    [[nodiscard]] b2WorldId getPhysicsWorld() const;

    /** The scale factor between the physics world, and display coordinates.
     * The physics world is measured in metres, whereas everything is displayed in pixels.
     *
     * IMPORTANT: Set this *before* adding any objects to the physics world, or use the default.
     * This will *NOT* rescale objects already in the world.
     */
    float worldScale{};

    /** Flags for debugging (can draw debug info to screen or print to the log).
     */
    unsigned debugFlags;

    /** Controls the number of physics velocity iterations.
     */
    int subStepCount;

    /** Controls the number of physics position iterations.
     */
    float timeStep;

private:

    shared_ptr<Command> playerAvatar;

    list<shared_ptr<Actor>> actors;

    /** The camera.
         */
    std::shared_ptr<TrackingCamera2D> camera;

     /** Our worldId .
    */
    b2WorldId physicsWorldId{};

    /** Object to draw the physics world (for debugging).
     *  Callback functions from the b2Box b2DebugDraw template
     */
    DrawPhysics drawPhysics;

    /** A flag indicating it's time to exit back to the menu.
    */
    bool exit;

};


