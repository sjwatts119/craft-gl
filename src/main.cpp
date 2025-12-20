#include <render/window.h>

#include "core/debug.h"
#include "core/interface.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE};
    window.makeCurrent();

    const RenderManager renderManager{&window};
    World world;
    Player player{&world};
    const Interface interface;
    Debug debug;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        player.update(&window);
        world.update();
        debug.update(&player, &world);

        renderManager.render(&player, &window, &world, &interface, &debug);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
