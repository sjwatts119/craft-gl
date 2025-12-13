#pragma once

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <iostream>

enum CameraMode
{
    FREE,
    FPS,
};

enum CameraDirection
{
    FORWARD,
    RIGHT,
    LEFT,
    BACKWARD,
    UP,
    DOWN,
};

class Camera
{
    CameraMode _mode;

    float _zoomSensitivity = 2.0f;
    float _aimSensitivity = 0.1f;
    float _movementSensitivity = 5.0f;

    float _pitch = 0.0f;
    float _yaw = -90.0f;
    float _fov = 45.0f;

    glm::vec3 _position{0.0f, 0.0f, 3.0f};

    glm::vec3 _worldUp{0.0f, 1.0f, 0.0f};

    glm::vec3 _forward{0.0f, 0.0f, -1.0f};
    glm::vec3 _up = _worldUp;
    glm::vec3 _right{1.0f, 0.0f, 0.0f};

    void moveForward(float speed);

    void moveBackward(float speed);

    void moveLeft(float speed);

    void moveRight(float speed);

    void moveDown(float speed);

    void moveUp(float speed);

public:
    explicit Camera(const CameraMode mode) : _mode(mode) {}

    void setBasisVectors();

    void aim(float yawOffset, float pitchOffset);

    void move(CameraDirection direction, float deltaTime);

    void zoom(float offset);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] float getFov() const;

    [[nodiscard]] glm::vec3 getPosition() const;
};
