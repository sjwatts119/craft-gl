#include <core/player.h>

void Player::update(const Window* window) {
    processCursor(window);
    processMouse(window);
    processKeyboard(window);

    clearAimingAtBlock();
    setAimingAtBlock(); // TODO cache highlighted block and don't regen mesh if it hasn't changed.
}

void Player::processCursor(const Window* window) {
    double xPosition, yPosition;

    glfwGetCursorPos(window->getWindow(), &xPosition, &yPosition);

    // Get the "amount" that the camera has moved since the last frame.
    // Order of operations is important as it "inverts" the input direction
    const float xOffset = static_cast<float>(xPosition) - mouseLastXPosition;
    const float yOffset = mouseLastYPosition - static_cast<float>(yPosition);

    // Store the current mouse position to calculate the next frame's offset.
    mouseLastXPosition = static_cast<float>(xPosition);
    mouseLastYPosition = static_cast<float>(yPosition);

    aim(xOffset, yOffset);
}

void Player::processMouse(const Window* window) {
    const bool mouse1IsPressed = glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

    // Prevent holding down the button
    if (mouse1IsPressed && !_mouse1WasPressed) {
        destroyHighlightedBlock();
    }

    _mouse1WasPressed = mouse1IsPressed;
}

void Player::processKeyboard(const Window* window) {
    /**
     * Movement
     */
    if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        move(FORWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        move(BACKWARD, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        move(LEFT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        move(RIGHT, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move(UP, window->getDeltaTime());
    }

    if (glfwGetKey(window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move(DOWN, window->getDeltaTime());
    }
}

void Player::moveForward(const float speed) {
    if (_mode == FREE) {
        _position += speed * _forward;

        return;
    }

    _position += glm::cross(_right, _worldUp) * speed;
}

void Player::moveBackward(const float speed) {
    if (_mode == FREE) {
        _position -= speed * _forward;

        return;
    }

    _position -= glm::cross(_right, _worldUp) * speed;
}

void Player::moveLeft(const float speed) {
    _position -= glm::cross(_forward, _up) * speed;
}

void Player::moveRight(const float speed) {
    _position += glm::cross(_forward, _up) * speed;
}

void Player::moveUp(const float speed) {
    _position += _worldUp * speed;
}

void Player::moveDown(const float speed) {
    _position -= _worldUp * speed;
}

void Player::setBasisVectors()
{
    glm::vec3 newCameraDirection;

    newCameraDirection.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    newCameraDirection.y = std::sin(glm::radians(_pitch));
    newCameraDirection.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

    _forward = newCameraDirection;
    _right = glm::normalize(glm::cross(_worldUp, _forward));
    _up = glm::cross(_forward, _right);
}

void Player::aim(float yawOffset, float pitchOffset)
{
    // std::cout << "aimed with yawOffset: " << yawOffset << " and pitchOffset: " << pitchOffset << std::endl;

    pitchOffset *= _aimSensitivity;
    yawOffset *= _aimSensitivity;

    // Constrain pitch to -89 -> 89
    _pitch = std::clamp(_pitch + pitchOffset, -89.0f, 89.0f);
    _yaw += yawOffset;

    setBasisVectors();
}

void Player::move(const CameraDirection direction, const float deltaTime)
{
    // std::cout << "moved " << direction << std::endl;
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

void Player::zoom(const float offset)
{
    auto newFov = _fov - (offset * _zoomSensitivity);

    newFov = std::clamp(newFov, 1.0f, 90.0f);

    _fov = newFov;
}

void Player::destroyHighlightedBlock() const {
    if (_highlightedBlockWorldCoordinate == std::nullopt) {
        return;
    }

    _world->destroyBlock(_highlightedBlockWorldCoordinate.value());
}

void Player::clearAimingAtBlock() {
    if (_highlightedBlockWorldCoordinate == std::nullopt) {
        return;
    }

    const auto chunk = _world->_chunks.find(_highlightedBlockWorldCoordinate.value().toChunkSpace());

    if (chunk == _world->_chunks.end()) {
        return;
    }

    chunk->second->_mesh->unsetHighlightedBlock();
    chunk->second->_mesh->markAsDirty();

    _highlightedBlockWorldCoordinate = std::nullopt;
}

Block* Player::setAimingAtBlock() {
    const Ray ray {_position, _forward};

    const auto playerChunkCoordinate = Coordinate{_position}.toChunkSpace();

    const auto chunk = _world->_chunks.find(playerChunkCoordinate);

    if (chunk == _world->_chunks.end()) {
        std::cerr << "Player not in a chunk or chunk with current coordinate not found" << std::endl;

        return nullptr;
    }

    Block* closestBlock = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    glm::ivec3 highlightedBlockIndex{-1};

    const auto chunkWorldPosition = chunk->first;

    // Start with the current chunk the player is in, then try surround chunks.
    for (int x = 0; x < Chunk::_size; x++) {
        for (int y = 0; y < Chunk::_size; y++) {
            for (int z = 0; z < Chunk::_size; z++) {
                Block& block = chunk->second->_blocks[x][y][z];

                // Air doesn't count as being aimed at.
                if (block.getType() == AIR) {
                    continue;
                }

                const auto intersectionDistance = Ray::distanceToAABB(AABB{Coordinate{chunkWorldPosition.x * 16 + x, chunkWorldPosition.y * 16 + y, chunkWorldPosition.z * 16 + z}}, ray);

                if (intersectionDistance == -1.0f) {
                    continue;
                }

                if (intersectionDistance >= closestDistance) {
                    continue;
                }

                if (intersectionDistance > _reach) {
                    continue;
                }

                closestBlock = &block;
                closestDistance = intersectionDistance;
                highlightedBlockIndex = {x, y, z};
            }
        }
    }

    if (closestBlock == nullptr) {
        _highlightedBlockWorldCoordinate = std::nullopt;
        return closestBlock;
    }

    _highlightedBlockWorldCoordinate = Coordinate{chunkWorldPosition.toIVec3() * Chunk::_size + highlightedBlockIndex};

    chunk->second->_mesh->setHighlightedBlock(highlightedBlockIndex);
    chunk->second->_mesh->markAsDirty();
    return closestBlock;
}

glm::mat4 Player::getViewMatrix() const
{
    return glm::lookAt(_position, _position + _forward, _up);
}

glm::mat4 Player::getProjectionMatrix(const int& width, const int& height) const
{
    return glm::mat4(glm::perspective(glm::radians(_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 250.0f));
}

glm::mat4 Player::getProjectionMatrix(const Window* window) const
{
    return getProjectionMatrix(window->getWidth(), window->getHeight());
}

float Player::getFov() const
{
    return _fov;
}

glm::vec3 Player::getPosition() const
{
    return _position;
}


