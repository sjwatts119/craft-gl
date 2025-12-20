#include <core/player.h>

#include "core/direction.h"

void Player::update(const Window* window) {
    processCursor(window);
    processMouse(window);
    processKeyboard(window);

    updateCameraPosition();
    updateBoundingBox();

    setAimingAtBlock();
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

void Player::processKeyboard(const Window* window) {
    /**
     * Movement
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        move(Direction::FORWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        move(Direction::BACKWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        move(Direction::LEFT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        move(Direction::RIGHT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move(Direction::UP, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move(Direction::DOWN, window->getDeltaTime());
    }
}

/**
 * MOVEMENT
 */
void Player::moveForward(const float speed) {
    _position += glm::cross(_camera._right, _camera._worldUp) * speed;
}

void Player::moveBackward(const float speed) {
    _position -= glm::cross(_camera._right, _camera._worldUp) * speed;
}

void Player::moveLeft(const float speed) {
    _position -= glm::cross(_camera._forward, _camera._up) * speed;
}

void Player::moveRight(const float speed) {
    _position += glm::cross(_camera._forward, _camera._up) * speed;
}

void Player::moveUp(const float speed) {
    _position += _camera._worldUp * speed;
}

void Player::moveDown(const float speed) {
    _position -= _camera._worldUp * speed;
}

void Player::move(const Direction direction, const float deltaTime)
{
    // std::cout << "moved " << direction << std::endl;
    const float speed = _movementSensitivity * deltaTime;

    switch (direction)
    {
        case Direction::FORWARD:
            moveForward(speed);
            break;

        case Direction::BACKWARD:
            moveBackward(speed);
            break;

        case Direction::LEFT:
            moveLeft(speed);
            break;

        case Direction::RIGHT:
            moveRight(speed);
            break;

        case Direction::UP:
            moveUp(speed);
            break;

        case Direction::DOWN:
            moveDown(speed);
            break;
    }
}

void Player::updateBoundingBox() {
    auto minX = _position.x - (_playerWidth / 2.0f);
    auto maxX = _position.x + (_playerWidth / 2.0f);

    auto minY = _position.y;
    auto maxY = _position.y + _playerHeight;

    auto minZ = _position.z - (_playerWidth / 2.0f);
    auto maxZ = _position.z + (_playerWidth / 2.0f);

    _boundingBox = {{minX, minY, minZ}, {maxX, maxY, maxZ}};
}

/**
 * AIMING & CAMERA
 */
void Player::updateCameraPosition()
{
    _camera._position = _position + glm::vec3{0.0f, _eyeHeight, 0.0f};
}

void Player::aim(float yawOffset, float pitchOffset)
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

    _world->placeBlock(_highlightedBlockWorldCoordinate.value(), _highlightedBlockFace.value());
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
                    if (glm::distance(static_cast<glm::vec3>(chunkWorldPosition + glm::ivec3(x, y, z)), _position) > _reach + 1) {
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