#include <render/window.h>

#include "core/camera.h"
#include "render/renderManager.h"

int main() {
    Window window{1920, 1080, "craft-gl"};
    window.makeCurrent();

    RenderManager renderManager{&window};
    renderManager.addTestBlocks();

    Camera camera{FPS};

    while (window.open()) {
        window.update();

        RenderManager::clear();

        camera.processCursor(&window);
        camera.processKeyboard(&window);

        renderManager.render(&camera, &window);

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
