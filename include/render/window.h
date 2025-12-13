#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
    int _width;
    int _height;

    std::string _title;

    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    GLFWwindow* _window;

    void initWindow();

    void updateDeltaTime();

public:
    Window(int width, int height, const std::string &title);

    void makeCurrent() const;

    void update();

    void poll();

    void swapBuffers();

    static void destroy();

    [[nodiscard]] bool open() const;

    [[nodiscard]] GLFWwindow* getWindow() const;
};
