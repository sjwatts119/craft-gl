#pragma once

#include <iostream>
#include <optional>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "core/world.h"
#include "render/window.h"
#include "geometry/chunkMesh.h"
#include "geometry/ray.h"
#include "render/window.h"

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

class Player
{
    CameraMode _mode;

    float _zoomSensitivity = 2.0f;
    float _aimSensitivity = 0.1f;
    float _movementSensitivity = 25.0f;

    float _pitch = 0.0f;
    float _yaw = 0.0f;
    float _fov = 75.0f;

    bool _firstMouseInput = true;
    float _mouseLastXPosition = SCREEN_WIDTH / 2.0f;
    float _mouseLastYPosition = SCREEN_HEIGHT / 2.0f;

    bool _mouse1WasPressed = false;
    bool _mouse2WasPressed = false;

    float _reach = 5.0f;

    glm::vec3 _position{0.0f, 38.0f, 0.0f};

    glm::vec3 _worldUp{0.0f, 1.0f, 0.0f};

    glm::vec3 _forward{0.0f, 0.0f, -1.0f};
    glm::vec3 _up = _worldUp;
    glm::vec3 _right{1.0f, 0.0f, 0.0f};

    World *_world;

    Block *_aimingAt;

    std::optional<Coordinate> _highlightedBlockWorldCoordinate;

    void moveForward(float speed);

    void moveBackward(float speed);

    void moveLeft(float speed);

    void moveRight(float speed);

    void moveDown(float speed);

    void moveUp(float speed);

public:
    explicit Player(const CameraMode mode, World *world) : _mode(mode), _world(world) {}

    void update(const Window* window);

    void processCursor(const Window* window);

    void processMouse(const Window* window);

    void processKeyboard(const Window* window);

    void setBasisVectors();

    void aim(float yawOffset, float pitchOffset);

    void move(CameraDirection direction, float deltaTime);

    void zoom(float offset);

    void destroyHighlightedBlock() const;

    void clearAimingAtBlock();

    [[nodiscard]] Block* setAimingAtBlock();

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(const int& width, const int& height) const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(const Window* window) const;

    [[nodiscard]] float getFov() const;

    [[nodiscard]] glm::vec3 getPosition() const;
};
