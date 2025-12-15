#include <render/window.h>

#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{1920, 1080, "craft-gl"};
    window.makeCurrent();

    RenderManager renderManager{&window};
    World world;
    Player player{FPS, &world};

    while (window.open()) {
        window.update();

        RenderManager::clear();

        player.update(&window);
        world.update();

        renderManager.render(&player, &window, &world);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
