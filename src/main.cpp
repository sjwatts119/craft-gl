#include <render/window.h>

#include "render/renderable/debug.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE};
    window.makeCurrent();

    RenderManager renderManager{&window};
    World world;
    Player player{&world};
    Debug debug;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        for (int i = 0; i < window.getTicksElapsed(); i++) {
            player.tick(&window);
            world.tick(&player);
        }

        world.update();
        player.update(&window);
        debug.update(&player, &world);

        renderManager.render(&player, &window, &world, &debug);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
