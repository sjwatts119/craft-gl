#pragma once

#include <iostream>
#include <optional>

#include "camera.h"
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "core/world.h"
#include "render/window.h"
#include "geometry/chunkMesh.h"
#include "geometry/ray.h"
#include "render/window.h"

enum class MovementMode {
    FLYING,
    WALKING,
};

class Player {
    /** Sensitivities and configurables **/
    MovementMode _mode = MovementMode::FLYING;
    float _zoomSensitivity = 2.0f;
    float _aimSensitivity = 0.1f;
    float _movementSensitivity = 25.0f;
    float _reach = 10.0f;

    /** Dimensions **/
    float _playerHeight = 1.8f;
    float _playerWidth = 0.6f;
    float _eyeHeight = _playerHeight - 0.2f;

    /** State **/
    bool _firstMouseInput = true;
    float _mouseLastXPosition = SCREEN_WIDTH / 2.0f;
    float _mouseLastYPosition = SCREEN_HEIGHT / 2.0f;
    bool _mouse1WasPressed = false;
    bool _mouse2WasPressed = false;
    Block *_aimingAt;
    std::optional<Coordinate> _highlightedBlockWorldCoordinate;
    std::optional<BlockFace> _highlightedBlockFace;
    AABB _boundingBox;

    /** Position **/
    glm::vec3 _position{0.0f, CHUNK_SIZE * 4 + 4.0f, 0.0f};

    glm::vec3 _momentum{0.0f};

    const glm::vec3 _acceleration{2.0f, 2.0f, 2.0f};

    float _groundResistance = 0.9f;

    Camera _camera{_position + glm::vec3{0.0f, _eyeHeight, 0.0f}};

    World *_world;

    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();

    void moveDown();

    void applyCollisions();

    void applyAcceleration(float deltaTime);

    void applyResistance(float deltaTime);

    void applyGravity(float deltaTime);

    void applyYCollisions();

    void applyYAcceleration(float deltaTime);

    void applyXAcceleration(float deltaTime);

    void moveUp();

    void updateCameraPosition();

    void aim(float yawOffset, float pitchOffset);

public:
    explicit Player(World *world): _aimingAt(nullptr), _world(world) {
    }

    void update(const Window *window);

    void processCursor(const Window *window);

    void processMouse(const Window *window);

    void processKeyboard(const Window *window);

    void applyZAcceleration(float deltaTime);

    void updateBoundingBox();

    void zoom(float offset);

    void destroyHighlightedBlock() const;

    void placeBlock() const;

    void clearAimingAtBlock();

    void setAimingAtBlock();

    [[nodiscard]] glm::vec3 getPosition() const;

    [[nodiscard]] AABB getBoundingBox() const {
        return _boundingBox;
    }

    [[nodiscard]] const Camera *getCamera() const {
        return &_camera;
    }
};
