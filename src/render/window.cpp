#include <render/window.h>

#include <utility>

#include "core/staticData.h"

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

    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

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
        _ticked = false;
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
    _sinceLastTick -= TIME_PER_TICK;
    _ticked = true;
}

bool Window::shouldTick() const {
    return _sinceLastTick >= TIME_PER_TICK;
}

bool Window::ticked() const {
    return _ticked;
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

GLFWwindow* Window::getWindow() const {
    return _window;
}
