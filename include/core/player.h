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
#include "render/renderable/chunkMesh.h"
#include "geometry/ray.h"
#include "render/window.h"
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

    /** Dimensions **/
    float _playerHeight = 1.8f;
    float _playerWidth = 0.6f;
    float _eyeHeight = _playerHeight - 0.2f;

    /** State **/
    bool _firstMouseInput = true;
    float _mouseLastXPosition = SCREEN_WIDTH / 2.0f;
    float _mouseLastYPosition = SCREEN_HEIGHT / 2.0f;

    bool _debug = false;

    std::optional<Coordinate> _highlightedBlockWorldCoordinate;
    std::optional<BlockFace> _highlightedBlockFace;
    AABB _boundingBox;

    bool _grounded = false;
    float _slip = BLOCK_SLIPPERINESS_FACTOR;

    BlockType _selectedBlockType = BlockType::STONE;

    /** Debounce States **/
    bool _mouse1WasPressed = false;
    bool _mouse2WasPressed = false;
    bool _tabWasPressed = false;
    bool _mWasPressed = false;

    /** Position **/
    glm::vec3 _position{0.0f, CHUNK_SIZE * 2 + 8, 0.0f};
    glm::vec3 _lastPosition = _position;

    glm::vec3 _momentum{0.0f};

    Camera _camera{_position + glm::vec3{0.0f, _eyeHeight, 0.0f}};

    World *_world;

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

    void updateCameraPosition(const Window* window);

    void aim(float yawOffset, float pitchOffset);

public:
    explicit Player(World *world): _world(world) {
    }

    void update(const Window *window);

    void tick(const Window *window);

    void processCursor(const Window *window);

    void processMouse(const Window *window);

    [[nodiscard]] float slipperinessAccelerationMultiplier() const;

    void processKeyboard(const Window *window);

    void updateBoundingBox();

    void zoom(float offset);

    void destroyHighlightedBlock() const;

    void placeBlock() const;

    void clearAimingAtBlock();

    void setAimingAtBlock();

    [[nodiscard]] glm::vec3 getPosition() const;

    [[nodiscard]] Coordinate getBlockCoordinate() const;

    [[nodiscard]] Coordinate getChunkCoordinate() const;

    [[nodiscard]] std::vector<Coordinate> getSurroundingChunkCoordinates() const ;

    [[nodiscard]] AABB getBoundingBox() const;

    [[nodiscard]] const Camera *getCamera() const;

    [[nodiscard]] bool getDebug() const;
};
