#pragma once

#include "Scene.hpp"
#include "Scarfy.hpp"

/** A scene for scarfy to walk around in.
 */
class ScarfyScene : public Scene {
public:
    ScarfyScene();

    virtual ~ScarfyScene();

    virtual void loadResources();
};

