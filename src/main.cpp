#include <render/window.h>

#include "core/interface.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE};
    window.makeCurrent();

    const RenderManager renderManager{&window};
    World world;
    Player player{FPS, &world};
    const Interface interface;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        player.update(&window);
        world.update();

        renderManager.render(&player, &window, &world, &interface);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
