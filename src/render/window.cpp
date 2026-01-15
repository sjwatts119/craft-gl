#include <render/window.h>

#include <utility>

#include "utility/constant.h"

Window::Window(
    const int width,
    const int height,
    std::string title
): _width(width), _height(height), _title(std::move(title)) {
    _aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    initWindow();
}

void Window::initWindow() {
    if (!glfwInit()) {
        throw std::logic_error("Failed to initialise GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto monitor = WINDOW_FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr;
    _window = glfwCreateWindow(_width, _height, _title.c_str(), monitor, nullptr);

    if (_window == nullptr) {
        glfwTerminate();
        throw std::logic_error("Failed to create GLFW window");
    }

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout << "Created Window" << std::endl;
}

void Window::updateDeltaTime() {
    const auto currentFrameElapsed = static_cast<float>(glfwGetTime());
    _deltaTime = currentFrameElapsed - _sinceLastFrame;
    _sinceLastFrame = currentFrameElapsed;
}

void Window::updateTickTime() {
    _sinceLastTick += _deltaTime;
}

void Window::makeCurrent() const {
    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::logic_error("Failed to initialise GLAD");
    }

    std::cout << "Made Window {" << _title << "} Current" << std::endl;
}

void Window::update() {
    updateDeltaTime();
    updateTickTime();

    if (shouldTick()) {
        tick();
    } else {
        _ticksElapsed = 0;
    }
}

void Window::poll() {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(_window);
}

void Window::destroy() {
    glfwTerminate();
}

bool Window::open() const {
    return !glfwWindowShouldClose(_window);
}

void Window::tick() {
    const auto ticks = static_cast<int>(std::floor(_sinceLastTick / TIME_PER_TICK));

    _ticksElapsed = std::min(ticks, MAX_TICKS_PER_FRAME);
    _sinceLastTick = _sinceLastTick - (_ticksElapsed * TIME_PER_TICK);
}

bool Window::shouldTick() const {
    return _sinceLastTick >= TIME_PER_TICK;
}

bool Window::ticked() const {
    return _ticksElapsed > 0;
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

float Window::getAspectRatio() const {
    return _aspectRatio;
}

float Window::getDeltaTime() const {
    return _deltaTime;
}

float Window::getTimeSinceLastTick() const {
    return _sinceLastTick;
}

int Window::getTicksElapsed() const {
    return _ticksElapsed;
}


GLFWwindow* Window::getWindow() const {
    return _window;
}
