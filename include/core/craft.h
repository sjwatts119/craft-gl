#pragma once

class Debug;
class Player;
class World;
class RenderManager;
class Window;

namespace Craft {
    extern Window *window;
    extern RenderManager *renderManager;
    extern World *world;
    extern Player *player;
    extern Debug *debug;

    void setup();

    void destroy();
}
