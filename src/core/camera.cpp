#include "core/camera.h"

#include "core/craft.h"
#include "geometry/AABB.h"

void Camera::updateBasisVectors() {
    glm::vec3 newCameraDirection;

    newCameraDirection.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    newCameraDirection.y = std::sin(glm::radians(_pitch));
    newCameraDirection.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

    _forward = newCameraDirection;
    _right = glm::normalize(glm::cross(_worldUp, _forward));
    _up = glm::cross(_forward, _right);
}

void Camera::move(const glm::vec3 newPosition) {
    _position = newPosition;
}

void Camera::aim(const float yawOffset, const float pitchOffset) {
    _pitch = std::clamp(_pitch + pitchOffset, -89.0f, 89.0f);
    _yaw += yawOffset;

    updateBasisVectors();
}

void Camera::zoom(const float offset) {
    const auto newFov = _fov - offset;

    _fov = std::clamp(newFov, 1.0f, 90.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _forward, _up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(
        glm::radians(_fov),
        static_cast<float>(Craft::window->getWidth()) / static_cast<float>(Craft::window->getHeight()),
        _nearClip,
        _farClip
    );
}