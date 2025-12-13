#include <render/window.h>

Window::Window(
    const int width,
    const int height,
    const std::string &title
): _width(width), _height(height), _title(title) {
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

    std::cout << "Created Window" << std::endl;
}

void Window::updateDeltaTime() {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;
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

GLFWwindow* Window::getWindow() const {
    return _window;
}
