#pragma once

#include "Scene.hpp"
#include "Scarfy.hpp"
#include "TileMap2DScene.hpp"


/** A scene for scarfy to walk around in.
 */
class ScarfyScene : public TileMap2DScene {
public:
    ScarfyScene();

    virtual ~ScarfyScene();

    virtual void loadResources();
};

