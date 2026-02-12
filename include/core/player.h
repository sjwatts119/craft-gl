#pragma once

#include <iostream>
#include <optional>

#include "camera.h"
#include "render/renderable/inventory.h"
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "core/world.h"
#include "render/window.h"
#include "render/renderable/chunkMesh.h"
#include "geometry/ray.h"
#include "render/window.h"
#include "render/renderable/crosshair.h"
#include "utility/direction.h"

enum class MovementMode {
    FLYING,
    WALKING,
};

class Player {
    /** Sensitivities and configurables **/
    MovementMode _mode = MovementMode::WALKING;
    float _zoomSensitivity = 2.0f;
    float _aimSensitivity = 0.1f;
    float _movementSensitivity = 25.0f;
    float _reach = 10.0f;

    /** State **/
    bool _firstMouseInput = true;
    float _mouseLastXPosition = Constant::WINDOW_WIDTH / 2.0f;
    float _mouseLastYPosition = Constant::WINDOW_HEIGHT / 2.0f;
    bool _debug = false;
    std::optional<Coordinate> _highlightedBlockWorldCoordinate;
    std::optional<BlockFace> _highlightedBlockFace;
    AABB _boundingBox;

    /** Debounce States **/
    bool _mouse1WasPressed = false;
    bool _mouse2WasPressed = false;
    bool _tabWasPressed = false;
    bool _mWasPressed = false;

    /** Dimensions **/
    float _playerHeight = 1.8f;
    float _playerWidth = 0.6f;
    float _eyeHeight = _playerHeight - 0.2f;

    /** Physics **/
    glm::vec3 _position{0.0f, Constant::CHUNK_SIZE * 3 + 8, 0.0f};
    glm::vec3 _lastPosition = _position;
    glm::vec3 _momentum{0.0f};
    bool _grounded = false;
    float _slip = Constant::BLOCK_SLIPPERINESS_FACTOR;

    /** Components **/
    Camera _camera{_position + glm::vec3{0.0f, _eyeHeight, 0.0f}};
    Inventory _inventory;
    Crosshair _crosshair;

    void moveForward(float accelerationMultiplier);

    void moveBackward(float accelerationMultiplier);

    void moveLeft(float accelerationMultiplier);

    void moveRight(float accelerationMultiplier);

    void moveDown(float accelerationMultiplier);

    void moveUp(float accelerationMultiplier);

    void jump();

    void flyUp(float accelerationMultiplier);

    void applyResistance();

    void applyGravity();

    void capMomentum();

    void updateLastPosition();

    void updatePosition();

    void updateSlip();

    void updateCameraPosition();

    void aim(float yawOffset, float pitchOffset);

public:
    void update();

    void tick();

    void processCursor();

    void processMouse();

    [[nodiscard]] float slipperinessAccelerationMultiplier() const;

    void processKeyboard();

    void updateBoundingBox();

    void respawn();

    void zoom(float offset);

    void destroyHighlightedBlock() const;

    void placeBlock() const;

    void clearAimingAtBlock();

    void setAimingAtBlock();

    [[nodiscard]] glm::vec3 getPosition() const;

    [[nodiscard]] Coordinate getBlockCoordinate() const;

    [[nodiscard]] Coordinate getChunkCoordinate() const;

    [[nodiscard]] std::vector<Coordinate> getSurroundingChunkCoordinates() const ;

    [[nodiscard]] bool isOutOfWorld() const;

    [[nodiscard]] AABB getBoundingBox() const;

    [[nodiscard]] const Camera *getCamera() const;

    [[nodiscard]] const Crosshair *getCrosshair() const;

    Inventory *getInventory();

    [[nodiscard]] bool debugIsEnabled() const;
};
