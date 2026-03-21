#include "core/craft.h"

#include "render/window.h"
#include "render/renderManager.h"
#include "core/world.h"
#include "core/player.h"

namespace Craft {
    Window *window;
    RenderManager *renderManager;
    World *world;
    Player *player;
    Debug *debug;

    void setup() {
        static Window _window;
        window = &_window;

        static RenderManager _renderManager;
        renderManager = &_renderManager;

        static Player _player;
        player = &_player;

        static World _world;
        world = &_world;
        world->init();

        static Debug _debug;
        debug = &_debug;
    }

    void destroy() {
        window->destroy();
    }
}