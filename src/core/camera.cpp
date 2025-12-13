#include <core/camera.h>

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
    pitchOffset *= _aimSensitivity;
    yawOffset *= _aimSensitivity;

    // Constrain pitch to -89 -> 89
    _pitch = std::clamp(_pitch + pitchOffset, -89.0f, 89.0f);
    _yaw += yawOffset;

    setBasisVectors();
}

void Camera::move(const CameraDirection direction, const float deltaTime)
{
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

float Camera::getFov() const
{
    return _fov;
}

glm::vec3 Camera::getPosition() const
{
    return _position;
}


