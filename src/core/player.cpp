#include <core/player.h>

#include "core/chunk.h"

void Player::update(const Window* window) {
    processCursor(window);

    updateCameraPosition(window);
    setAimingAtBlock();
}

void Player::tick(const Window* window) {
    updateLastPosition();

    processMouse(window);
    processKeyboard(window);

    updateSlip();
    updatePosition();

    applyGravity();
    applyResistance();
    capMomentum();
}

/**
 * INPUT HANDLING
 */
void Player::processCursor(const Window* window) {
    double xPosition, yPosition;
    glfwGetCursorPos(window->getWindow(), &xPosition, &yPosition);

    // Prevent jerky first frame from huge offset when user clicks in the window.
    if (_firstMouseInput) {
        _mouseLastXPosition = static_cast<float>(xPosition);
        _mouseLastYPosition = static_cast<float>(yPosition);
        _firstMouseInput = false;
    }

    // Get the "amount" that the camera has moved since the last frame.
    // Order of operations is important as it "inverts" the input direction
    const float xOffset = static_cast<float>(xPosition) - _mouseLastXPosition;
    const float yOffset = _mouseLastYPosition - static_cast<float>(yPosition);

    // Store the current mouse position to calculate the next frame's offset.
    _mouseLastXPosition = static_cast<float>(xPosition);
    _mouseLastYPosition = static_cast<float>(yPosition);

    aim(xOffset, yOffset);
}

void Player::processMouse(const Window* window) {
    /** Left mouse button **/
    const bool mouse1IsPressed = glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

    // Prevent holding down the button
    if (mouse1IsPressed && !_mouse1WasPressed) {
        destroyHighlightedBlock();
    }

    _mouse1WasPressed = mouse1IsPressed;

    /** Right mouse button **/
    const bool mouse2IsPressed = glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

    // Prevent holding down the button
    if (mouse2IsPressed && !_mouse2WasPressed) {
        placeBlock();
    }

    _mouse2WasPressed = mouse2IsPressed;
}

float Player::slipperinessAccelerationMultiplier() const {
    if (_mode == MovementMode::FLYING) {
        return WALKING_ACCELERATION;
    }

    if (!_grounded) {
        return NO_SLIPPERINESS;
    }

    constexpr auto baseSlip = BLOCK_SLIPPERINESS_FACTOR * HORIZONTAL_RESISTANCE_FACTOR;
    const auto currentSlip = _slip * HORIZONTAL_RESISTANCE_FACTOR;

    const auto accelerationRatio = (baseSlip / currentSlip);
    const auto accelerationMultiplier = accelerationRatio * accelerationRatio * accelerationRatio;

    return WALKING_ACCELERATION * accelerationMultiplier;
}

void Player::processKeyboard(const Window* window) {
    const auto accelerationMultiplier = slipperinessAccelerationMultiplier();

    /**
     * Movement
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        moveForward(accelerationMultiplier);
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        moveBackward(accelerationMultiplier);
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        moveLeft(accelerationMultiplier);
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        moveRight(accelerationMultiplier);
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        moveUp(accelerationMultiplier);
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        moveDown(accelerationMultiplier);
    }

    /**
     * Block selection
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(0));
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(1));
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(2));
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(3));
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(4));
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
    {
        _inventory.selectBlockType(static_cast<BlockType>(5));
    }

    // TODO track dirty state to avoid regenerating every frame
    _inventory.regenerateMesh();
    _inventory.upload();

    // if (glfwGetKey(window->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
    // {
    //     _inventory.selectBlockType(static_cast<BlockType>(6));
    // }
    // if (glfwGetKey(window->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
    // {
    //     _inventory.selectBlockType(static_cast<BlockType>(7));
    // }
    // if (glfwGetKey(window->getWindow(), GLFW_KEY_9) == GLFW_PRESS)
    // {
    //     _inventory.selectBlockType(static_cast<BlockType>(8));
    // }

    /**
     * Mode toggles
     */
    const auto mIsPressed = glfwGetKey(window->getWindow(), GLFW_KEY_M) == GLFW_PRESS;
    if (mIsPressed && !_mWasPressed) {
        _mode = _mode == MovementMode::WALKING ? MovementMode::FLYING : MovementMode::WALKING;
    }
    _mWasPressed = mIsPressed;

    const auto tabIsPressed = glfwGetKey(window->getWindow(), GLFW_KEY_TAB) == GLFW_PRESS;
    if (tabIsPressed && !_tabWasPressed) {
        _debug = !_debug;
    }
    _tabWasPressed = tabIsPressed;

    /**
     * Kill window
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window->getWindow(), true);
    }
}

/**
 * MOVEMENT
 */
void Player::moveForward(const float accelerationMultiplier) {
    _momentum += glm::cross(_camera._right, _camera._worldUp) * accelerationMultiplier;
}

void Player::moveBackward(const float accelerationMultiplier) {
    _momentum -= glm::cross(_camera._right, _camera._worldUp) * accelerationMultiplier;
}

void Player::moveLeft(const float accelerationMultiplier) {
    _momentum -= glm::cross(_camera._forward, _camera._up) * accelerationMultiplier;
}

void Player::moveRight(const float accelerationMultiplier) {
    _momentum += glm::cross(_camera._forward, _camera._up) * accelerationMultiplier;
}

void Player::moveUp(const float accelerationMultiplier) {
    _mode == MovementMode::FLYING ? flyUp(accelerationMultiplier) : jump();
}

void Player::jump() {
    if (!_grounded) {
        return;
    }

    _momentum.y = JUMP_VELOCITY;
}

void Player::flyUp(const float accelerationMultiplier) {
    _momentum += _camera._worldUp * accelerationMultiplier;
}

void Player::moveDown(const float accelerationMultiplier) {
    if (_mode != MovementMode::FLYING) {
        return;
    }

    _momentum -= _camera._worldUp * accelerationMultiplier;
}

void Player::applyGravity() {
    if (_mode == MovementMode::FLYING) {
        return;
    }

    _momentum.y -= GRAVITY_ACCELERATION;
}

void Player::applyResistance() {
    if (_mode == MovementMode::FLYING) {
        _momentum *= FLYING_RESISTANCE_FACTOR;

        return;
    }

    const auto horizontalResistanceFactor = _grounded
        ? _slip * HORIZONTAL_RESISTANCE_FACTOR
        : HORIZONTAL_RESISTANCE_FACTOR;

    _momentum.x *= horizontalResistanceFactor;
    _momentum.y *= VERTICAL_RESISTANCE_FACTOR;
    _momentum.z *= horizontalResistanceFactor;
}

void Player::capMomentum() {
    _momentum = glm::clamp(
        _momentum,
        -glm::vec3{TERMINAL_VELOCITY},
        glm::vec3{TERMINAL_VELOCITY}
    );

    if (std::abs(_momentum.x) < EPSILON) {
        _momentum.x = 0.0f;
    }

    if (std::abs(_momentum.y) < EPSILON) {
        _momentum.y = 0.0f;
    }

    if (std::abs(_momentum.z) < EPSILON) {
        _momentum.z = 0.0f;
    }
}

void Player::updateLastPosition() {
    _lastPosition = _position;
}

void Player::updatePosition() {
    const auto oldPosition = _position;
    const auto newPosition = _position + _momentum;

    auto movementAABB = _boundingBox;
    movementAABB.expandTo(newPosition);

    // Distance to push out testable coordinates based on width/height of player
    const glm::vec3 push {_playerWidth / 2, _playerHeight / 2, _playerWidth / 2};

    const int minX = static_cast<int>(std::floor(movementAABB.minX - push.x));
    const int maxX = static_cast<int>(std::floor(movementAABB.maxX + push.x));
    const int minY = static_cast<int>(std::floor(movementAABB.minY - push.y));
    const int maxY = static_cast<int>(std::floor(movementAABB.maxY + push.y));
    const int minZ = static_cast<int>(std::floor(movementAABB.minZ - push.z));
    const int maxZ = static_cast<int>(std::floor(movementAABB.maxZ + push.z));

    // Get all blocks that could collide with the player during movement.
    std::vector<AABB> testableBlocks;
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                auto block = _world->blockAt(Coordinate{x, y, z});

                if (block == nullptr || block->getType() == BlockType::AIR) {
                    continue;
                }

                testableBlocks.push_back(AABB::forBlock(Coordinate{x, y, z}));
            }
        }
    }

    auto deltaX = newPosition.x - oldPosition.x;
    auto deltaY = newPosition.y - oldPosition.y;
    auto deltaZ = newPosition.z - oldPosition.z;
    const auto originalDeltaX = deltaX;
    const auto originalDeltaY = deltaY;
    const auto originalDeltaZ = deltaZ;

    // clip movement on X
    for (const auto& testableBlock : testableBlocks) {
        deltaX = _boundingBox.clipX(testableBlock, deltaX);
    }
    _position.x += deltaX;
    _momentum.x = (originalDeltaX != deltaX) ? 0.0f : _momentum.x;
    updateBoundingBox();

    // clip movement on Y
    for (const auto& testableBlock : testableBlocks) {
        deltaY = _boundingBox.clipY(testableBlock, deltaY);
    }
    _position.y += deltaY;
    _grounded = (originalDeltaY != deltaY && originalDeltaY < 0.0f);
    _momentum.y = (originalDeltaY != deltaY) ? 0.0f : _momentum.y;
    updateBoundingBox();

    // clip movement on Z
    for (const auto& testableBlock : testableBlocks) {
        deltaZ = _boundingBox.clipZ(testableBlock, deltaZ);
    }
    _position.z += deltaZ;
    _momentum.z = (originalDeltaZ != deltaZ) ? 0.0f : _momentum.z;
    updateBoundingBox();
}

void Player::updateSlip() {
    if (!_grounded) {
        _slip = BLOCK_SLIPPERINESS_FACTOR;
        return;
    }

    constexpr auto verticalPush = glm::vec3{0.0f, STANDING_ON_NEGATIVE_Y_OFFSET, 0.0f};
    auto pushedAABB = _boundingBox;
    pushedAABB.push(verticalPush);

    const auto blockCoord = getBlockCoordinate();
    const auto directlyBelowCoord = Coordinate{
        blockCoord.x,
        blockCoord.y - 1,
        blockCoord.z
    };

    if (const auto blockBelow = _world->blockAt(directlyBelowCoord); blockBelow != nullptr && blockBelow->getType() != BlockType::AIR) {
        if (pushedAABB.intersects(AABB::forBlock(directlyBelowCoord))) {
            _slip = blockBelow->getSlipperinessFactor();
            return;
        }
    }

    const int minX = static_cast<int>(std::floor(pushedAABB.minX));
    const int maxX = static_cast<int>(std::floor(pushedAABB.maxX));
    const int minZ = static_cast<int>(std::floor(pushedAABB.minZ));
    const int maxZ = static_cast<int>(std::floor(pushedAABB.maxZ));
    const int checkY = static_cast<int>(std::floor(_position.y + STANDING_ON_NEGATIVE_Y_OFFSET));

    // spin around the bottom of the player bounding box to find the block they are standing on
    // start with the block entirely under the player and expand outwards
    for (int x = minX; x <= maxX; ++x) {
        for (int z = minZ; z <= maxZ; ++z) {
            if (Coordinate coord{x, checkY, z}; pushedAABB.intersects(AABB::forBlock(coord))) {
                if (const auto block = _world->blockAt(coord); block != nullptr && block->getType() != BlockType::AIR) {
                    _slip = block->getSlipperinessFactor();
                    return;
                }
            }
        }
    }

    _slip = BLOCK_SLIPPERINESS_FACTOR;
}

void Player::updateBoundingBox() {
    _boundingBox = AABB::forPlayer(_position, _playerWidth, _playerHeight);
}

/**
 * AIMING & CAMERA
 */
void Player::updateCameraPosition(const Window* window)
{
    const auto interpolationFactor = std::clamp(window->getTimeSinceLastTick() / TIME_PER_TICK, 0.0f, 1.0f);
    _camera._position = glm::mix(_lastPosition, _position, interpolationFactor) + glm::vec3{0.0f, _eyeHeight, 0.0f};
}

void Player::aim(const float yawOffset, const float pitchOffset)
{
    _camera.aim(yawOffset * _aimSensitivity, pitchOffset * _aimSensitivity);
}

void Player::zoom(const float offset)
{
    _camera.zoom(offset * _zoomSensitivity);
}

/**
 * BLOCK INTERACTION
 */
void Player::destroyHighlightedBlock() const {
    if (_highlightedBlockWorldCoordinate == std::nullopt) {
        return;
    }

    _world->destroyBlock(_highlightedBlockWorldCoordinate.value());
}

void Player::placeBlock() const {
    if (_highlightedBlockWorldCoordinate == std::nullopt || _highlightedBlockFace == std::nullopt) {
        return;
    }

    // push the coordinate to the position of the new block based on highlighted face
    const auto newWorldCoordinate = _highlightedBlockWorldCoordinate.value().moveTowards(_highlightedBlockFace.value());

    if (AABB::forBlock(newWorldCoordinate).intersects(_boundingBox)) {
        std::cout << "Cannot place block inside player bounding box" << std::endl;

        return;
    }

    _world->placeBlock(newWorldCoordinate, _inventory.getSelectedBlockType());
}


void Player::clearAimingAtBlock() {
    // player is not aiming at a block
    if (_highlightedBlockWorldCoordinate == std::nullopt || _highlightedBlockFace == std::nullopt) {
        _highlightedBlockWorldCoordinate = std::nullopt;
        _highlightedBlockFace = std::nullopt;
        return;
    }

    const auto chunk = _world->_chunks.find(_highlightedBlockWorldCoordinate.value().toChunkFromWorld());

    // chunk is out of bounds or not found
    if (chunk == _world->_chunks.end()) {
        _highlightedBlockWorldCoordinate = std::nullopt;
        _highlightedBlockFace = std::nullopt;
        return;
    }

    chunk->second->_mesh->unsetHighlightedBlock();
    chunk->second->_mesh->markAsDirty();

    _highlightedBlockWorldCoordinate = std::nullopt;
    _highlightedBlockFace = std::nullopt;
}

void Player::setAimingAtBlock() {
    Ray ray {_camera._position, _camera._forward};
    const auto traversed = ray.traversedCoordinates(10.0f);

    std::optional<Coordinate> aimedAtCoordinate;

    for (const auto& coord : traversed) {
        if (const auto block = _world->blockAt(coord); block == nullptr || block->getType() == BlockType::AIR) {
            continue;
        }

        aimedAtCoordinate = coord;
        break;
    }

    if (aimedAtCoordinate == std::nullopt) {
        clearAimingAtBlock();
        return;
    }

    const auto containingChunk = _world->_chunks.find(aimedAtCoordinate.value().toChunkFromWorld());
    if (containingChunk == _world->_chunks.end()) {
        clearAimingAtBlock();
        return;
    }

    const auto blockAABB = AABB::forBlock(aimedAtCoordinate.value());
    auto hitFace = Ray::getHitFace(blockAABB, ray, Ray::distanceToAABB(blockAABB, ray));

    // don't regenerate the mesh if the highlighted block hasn't changed
    if (_highlightedBlockWorldCoordinate == aimedAtCoordinate.value() && _highlightedBlockFace == hitFace) {
        return;
    }

    clearAimingAtBlock();

    _highlightedBlockWorldCoordinate = aimedAtCoordinate;
    _highlightedBlockFace = hitFace;
    containingChunk->second->_mesh->setHighlightedBlock(aimedAtCoordinate.value().toLocalFromWorld().toIVec3());
}

/**
 * GETTERS
 */
glm::vec3 Player::getPosition() const
{
    return _position;
}

Coordinate Player::getBlockCoordinate() const {
    return Coordinate{
        static_cast<int>(std::floor(_position.x)),
        static_cast<int>(std::floor(_position.y)),
        static_cast<int>(std::floor(_position.z))
    };
}

Coordinate Player::getChunkCoordinate() const {
    return getBlockCoordinate().toChunkFromWorld();
}

std::vector<Coordinate> Player::getSurroundingChunkCoordinates() const {
    std::vector<Coordinate> visibleChunks;

    const auto playerChunkCoordinate = getChunkCoordinate();

    // TODO this should not be a cube, but a sphere
    for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++) {
        for (int z = -RENDER_DISTANCE; z <= RENDER_DISTANCE; z++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                visibleChunks.emplace_back(
                    playerChunkCoordinate.x + x,
                    y,
                    playerChunkCoordinate.z + z
                );
            }
        }
    }


    return visibleChunks;
}

AABB Player::getBoundingBox() const {
    return _boundingBox;
}

const Camera *Player::getCamera() const {
    return &_camera;
}

const Crosshair *Player::getCrosshair() const {
    return &_crosshair;
}

const Inventory *Player::getInventory() const {
    return &_inventory;
}

bool Player::debugIsEnabled() const {
    return _debug;
}
