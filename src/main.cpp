#include <render/window.h>

#include "render/renderManager.h"

int main() {
    Window window{1920, 1080, "craft-gl"};
    window.makeCurrent();

    RenderManager renderManager;
    renderManager.addTestBlocks();

    while (window.open()) {
        window.update();

        Window::poll();
        window.swapBuffers();
    }

    Window::destroy();
}
