#include <render/window.h>

#include "render/renderable/debug.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"
#include "core/craft.h"

int main() {
    Window window{WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE};
    Craft::window = &window;
    Craft::window->makeCurrent();

    RenderManager renderManager{&window};
    Craft::renderManager = &renderManager;

    World world;
    Craft::world = &world;

    Player player{&world};
    Craft::player = &player;

    Debug debug;
    Craft::debug = &debug;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        for (int i = 0; i < window.getTicksElapsed(); i++) {
            player.tick();
            world.tick();
        }

        world.update();
        player.update();
        debug.update();

        renderManager.render();

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
