#include <core/camera.h>

#include "glm/ext/matrix_clip_space.hpp"
#include "render/window.h"

void Camera::processCursor(const Window* window) {
    double xPosition, yPosition;

    glfwGetCursorPos(window->getWindow(), &xPosition, &yPosition);

    // Get the "amount" that the camera has moved since the last frame.
    // Order of operations is important as it "inverts" the input direction
    const float xOffset = static_cast<float>(xPosition) - mouseLastXPosition;
    const float yOffset = mouseLastYPosition - static_cast<float>(yPosition);

    // Store the current mouse position to calculate the next frame's offset.
    mouseLastXPosition = static_cast<float>(xPosition);
    mouseLastYPosition = static_cast<float>(yPosition);

    aim(xOffset, yOffset);
}

void Camera::processKeyboard(const Window* window) {
    /**
     * Movement
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        move(FORWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        move(BACKWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        move(RIGHT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        move(LEFT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move(UP, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move(DOWN, window->getDeltaTime());
    }
}


void Camera::moveForward(const float speed) {
    if (_mode == FREE) {
        _position += speed * _forward;

        return;
    }

    _position += glm::cross(_right, _worldUp) * speed;
}

void Camera::moveBackward(const float speed) {
    if (_mode == FREE) {
        _position -= speed * _forward;

        return;
    }

    _position -= glm::cross(_right, _worldUp) * speed;
}

void Camera::moveLeft(const float speed) {
    _position += glm::cross(_forward, _up) * speed;
}

void Camera::moveRight(const float speed) {
    _position -= glm::cross(_forward, _up) * speed;
}

void Camera::moveUp(const float speed) {
    _position += _worldUp * speed;
}

void Camera::moveDown(const float speed) {
    _position -= _worldUp * speed;
}

void Camera::setBasisVectors()
{
    glm::vec3 newCameraDirection;

    newCameraDirection.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    newCameraDirection.y = std::sin(glm::radians(_pitch));
    newCameraDirection.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

    _forward = newCameraDirection;
    _right = glm::normalize(glm::cross(_worldUp, _forward));
    _up = glm::cross(_forward, _right);
}

void Camera::aim(float yawOffset, float pitchOffset)
{
    // std::cout << "aimed with yawOffset: " << yawOffset << " and pitchOffset: " << pitchOffset << std::endl;

    pitchOffset *= _aimSensitivity;
    yawOffset *= _aimSensitivity;

    // Constrain pitch to -89 -> 89
    _pitch = std::clamp(_pitch + pitchOffset, -89.0f, 89.0f);
    _yaw += yawOffset;

    setBasisVectors();
}

void Camera::move(const CameraDirection direction, const float deltaTime)
{
    // std::cout << "moved " << direction << std::endl;
    const float speed = _movementSensitivity * deltaTime;

    switch (direction)
    {
        case FORWARD:
            moveForward(speed);
            break;

        case BACKWARD:
            moveBackward(speed);
            break;

        case LEFT:
            moveLeft(speed);
            break;

        case RIGHT:
            moveRight(speed);
            break;

        case UP:
            moveUp(speed);
            break;

        case DOWN:
            moveDown(speed);
            break;
    }

    setBasisVectors();
}

void Camera::zoom(const float offset)
{
    auto newFov = _fov - (offset * _zoomSensitivity);

    newFov = std::clamp(newFov, 1.0f, 90.0f);

    _fov = newFov;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(_position, _position + _forward, _up);
}

glm::mat4 Camera::getProjectionMatrix(const int& width, const int& height) const
{
    return glm::mat4(glm::perspective(glm::radians(_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f));
}

glm::mat4 Camera::getProjectionMatrix(const Window* window) const
{
    return getProjectionMatrix(window->getWidth(), window->getHeight());
}

float Camera::getFov() const
{
    return _fov;
}

glm::vec3 Camera::getPosition() const
{
    return _position;
}


