#include <render/window.h>

int main() {
    Window window{1920, 1080, "craft-gl"};

    window.makeCurrent();

    while (window.open()) {
        window.update();

        window.poll();
        window.swapBuffers();
    }

    Window::destroy();
}
