#include <render/window.h>

#include "render/renderable/debug.h"
#include "core/interface.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE};
    window.makeCurrent();

    RenderManager renderManager{&window};
    World world;
    Player player{&world};
    Interface interface;
    Debug debug;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        for (int i = 0; i < window.getTicksElapsed(); i++) {
            player.tick(&window);
        }

        player.update(&window);
        world.update(&player);
        debug.update(&player, &world);

        renderManager.render(&player, &window, &world, &interface, &debug);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
