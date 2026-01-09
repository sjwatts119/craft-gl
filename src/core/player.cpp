#include <core/player.h>

void Player::update(const Window* window) {
    processCursor(window);

    updateCameraPosition(window);
    setAimingAtBlock();
}

void Player::tick(const Window* window) {
    updateLastPosition();

    processMouse(window);
    processKeyboard(window);

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
    // TODO: get actual block slipperiness
    constexpr auto currentSlip = BLOCK_SLIPPERINESS_FACTOR * HORIZONTAL_RESISTANCE_FACTOR;

    constexpr auto accelerationRatio = (baseSlip / currentSlip);
    constexpr auto accelerationMultiplier = accelerationRatio * accelerationRatio * accelerationRatio;

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

    if (glfwGetKey(window->getWindow(), GLFW_KEY_M) == GLFW_PRESS) {
        _mode = _mode == MovementMode::WALKING ? MovementMode::FLYING : MovementMode::WALKING;
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_TAB) == GLFW_PRESS) {
        _debug = !_debug;
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
        ? BLOCK_SLIPPERINESS_FACTOR * HORIZONTAL_RESISTANCE_FACTOR
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

                if (block == nullptr || block->getType() == AIR) {
                    continue;
                }

                testableBlocks.push_back(AABB::forBlock(Coordinate{x, y, z}));
            }
        }
    }

    float deltaX = newPosition.x - oldPosition.x;
    float deltaY = newPosition.y - oldPosition.y;
    float deltaZ = newPosition.z - oldPosition.z;
    float originalDeltaY = deltaY;

    // clip movement on X
    for (const auto& testableBlock : testableBlocks) {
        deltaX = _boundingBox.clipX(testableBlock, deltaX);
    }
    _position.x += deltaX;
    updateBoundingBox();

    // clip movement on Y
    for (const auto& testableBlock : testableBlocks) {
        deltaY = _boundingBox.clipY(testableBlock, deltaY);
    }
    _position.y += deltaY;

    _grounded = (originalDeltaY != deltaY && originalDeltaY < 0.0f);
    if (_grounded) {
        _momentum.y = 0.0f;
    }
    updateBoundingBox();

    // clip movement on Z
    for (const auto& testableBlock : testableBlocks) {
        deltaZ = _boundingBox.clipZ(testableBlock, deltaZ);
    }
    _position.z += deltaZ;
    updateBoundingBox();
}

void Player::updateBoundingBox() {
    _boundingBox = AABB::forPlayer(_position, _playerWidth, _playerHeight);
}

/**
 * AIMING & CAMERA
 */
void Player::updateCameraPosition(const Window* window)
{
    auto interpolationFactor = static_cast<float>(window->getTimeSinceLastTick()) / TIME_PER_TICK;
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

    _world->placeBlock(newWorldCoordinate);
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
    const auto playerChunkCoordinate = Coordinate{_position}.toChunkFromWorld();
    const auto currentChunkAttempt = _world->_chunks.find(playerChunkCoordinate);

    if (currentChunkAttempt == _world->_chunks.end()) {
        std::cerr << "Player not in a chunk or chunk with current coordinate not found" << std::endl;

        clearAimingAtBlock();
        return;
    }

    std::vector<Coordinate> testableChunkCoordinates;

    // Get current chunk and also surrounding chunks in 3 x 3 x 3 area
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                testableChunkCoordinates.emplace_back(playerChunkCoordinate.x + x, playerChunkCoordinate.y + y, playerChunkCoordinate.z + z);
            }
        }
    }

    const Ray ray {_camera._position, _camera._forward};
    Block* closestBlock = nullptr;
    std::optional<BlockFace> hitFace;
    float closestDistance = std::numeric_limits<float>::max();
    glm::ivec3 highlightedBlockIndex{-1};

    std::optional<Coordinate> aimedAtChunkCoordinate;
    const Chunk* aimedAtChunk = nullptr;

    // Test current and surrounding chunks
    for (auto& testableChunkCoordinate : testableChunkCoordinates) {
        const auto testableChunkAttempt = _world->_chunks.find(testableChunkCoordinate);

        // chunk is out of bounds or not found
        if (testableChunkAttempt == _world->_chunks.end()) {
            continue;
        }

        Chunk* testableChunk = testableChunkAttempt->second.get();

        // intersection test the ray with a bounding box on the entire chunk, if no collision, avoid testing any blocks.
        const auto chunkAABB = AABB::forChunk({Coordinate{testableChunkCoordinate.x, testableChunkCoordinate.y, testableChunkCoordinate.z}});

        if (Ray::distanceToAABB(chunkAABB, ray) == -1.0f) {
            continue;
        }

        const auto chunkWorldPosition = testableChunkCoordinate.toWorldFromChunk().toIVec3();

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    Block& block = testableChunk->_blocks[x][y][z];

                    // air doesn't count as being aimed at.
                    if (block.getType() == AIR) {
                        continue;
                    }

                    // block is at least a block away from player reach so early out
                    // we do more precise distance checks after ray-AABB intersection test
                    auto blockCenter = static_cast<glm::vec3>(chunkWorldPosition + glm::ivec3(x, y, z)) + glm::vec3(0.5f);
                    if (glm::distance(blockCenter, _position) > _reach + 1) {
                        continue;
                    }

                    const auto intersectionDistance = Ray::distanceToAABB(AABB::forBlock({Coordinate{chunkWorldPosition.x + x, chunkWorldPosition.y + y, chunkWorldPosition.z + z}}), ray);

                    // closer intersection already found
                    if (intersectionDistance >= closestDistance) {
                        continue;
                    }

                    // no intersection
                    if (intersectionDistance == -1.0f) {
                        continue;
                    }

                    // intersection is out of player reach
                    if (intersectionDistance > _reach) {
                        continue;
                    }

                    // store block we are aiming it
                    closestBlock = &block;
                    closestDistance = intersectionDistance;
                    highlightedBlockIndex = {x, y, z};
                    aimedAtChunk = testableChunk;
                    aimedAtChunkCoordinate = testableChunkCoordinate;
                    hitFace = Ray::getHitFace(AABB::forBlock({Coordinate{chunkWorldPosition.x + x, chunkWorldPosition.y + y, chunkWorldPosition.z + z}}), ray, intersectionDistance);
                }
            }
        }
    }

    // no intersection found, return nullptr
    if (closestBlock == nullptr || aimedAtChunk == nullptr) {
        clearAimingAtBlock();

        return;
    }

    auto worldCoordinate = aimedAtChunkCoordinate.value().toWorldFromChunk(Coordinate{highlightedBlockIndex});

    // don't regenerate the mesh if the highlighted block hasn't changed
    if (_highlightedBlockWorldCoordinate == worldCoordinate && _highlightedBlockFace == hitFace) {
        return;
    }

    clearAimingAtBlock();

    _highlightedBlockWorldCoordinate = worldCoordinate;
    _highlightedBlockFace = hitFace;

    aimedAtChunk->_mesh->setHighlightedBlock(highlightedBlockIndex);
    aimedAtChunk->_mesh->markAsDirty();
}

/**
 * GETTERS
 */
glm::vec3 Player::getPosition() const
{
    return _position;
}

AABB Player::getBoundingBox() const {
    return _boundingBox;
}

const Camera *Player::getCamera() const {
    return &_camera;
}

bool Player::getDebug() const {
    return _debug;
}
