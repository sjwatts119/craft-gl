#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
    int _width;
    int _height;
    float _aspectRatio;

    std::string _title;

    float _deltaTime = 0.0f;
    float _sinceLastFrame = 0.0f;

    int _tick = 0;
    float _sinceLastTick = 0.0f;
    bool _ticked = false;

    GLFWwindow *_window;

    void initWindow();

    void updateDeltaTime();

    void updateTickTime();

public:
    Window(int width, int height, std::string title);

    void makeCurrent() const;

    void update();

    void tick();

    bool shouldTick() const;

    bool ticked() const;

    static void poll();

    void swapBuffers() const;

    static void destroy();

    [[nodiscard]] bool open() const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] float getAspectRatio() const;

    [[nodiscard]] float getDeltaTime() const;

    [[nodiscard]] float getTimeSinceLastTick() const;

    [[nodiscard]] GLFWwindow *getWindow() const;
};
