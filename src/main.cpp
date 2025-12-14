#include <render/window.h>

#include "core/camera.h"
#include "core/world.h"
#include "render/renderManager.h"

int main() {
    Window window{1920, 1080, "craft-gl"};
    window.makeCurrent();

    RenderManager renderManager{&window};
    Camera camera{FPS};
    World world;

    while (window.open()) {
        window.update();

        RenderManager::clear();

        camera.processCursor(&window);
        camera.processKeyboard(&window);

        renderManager.render(&camera, &window, &world);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
