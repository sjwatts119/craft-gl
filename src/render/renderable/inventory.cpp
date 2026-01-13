#include "render/renderable/inventory.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "core/block.h"
#include "geometry/AABB.h"
#include "render/buffer/blockData.h"

Inventory::Inventory() {
    genBuffers();
    regenerateMesh();
    upload();
}

BlockType Inventory::getSelectedBlockType() const {
    return _selectedBlockType;
}

void Inventory::selectBlockType(const BlockType type) {
    _selectedBlockType = type;
}

void Inventory::selectNextBlockType() {
    auto nextType = static_cast<int>(_selectedBlockType) + 1;

    if (nextType >= static_cast<int>(BlockType::COUNT)) {
        nextType = 0;
    }

    _selectedBlockType = static_cast<BlockType>(nextType);
}

void Inventory::selectPreviousBlockType() {
    auto previousType = static_cast<int>(_selectedBlockType) - 1;

    if (previousType < 0) {
        previousType = static_cast<int>(BlockType::COUNT) - 1;
    }

    _selectedBlockType = static_cast<BlockType>(previousType);
}

void Inventory::regenerateMesh() {
    const auto block = Block{_selectedBlockType};

    _vertices = block.getBufferVertices();
    _indices = Block::getBufferIndices();
}

void Inventory::upload() {
    glBindVertexArray(_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(BlockData::size() * _vertices.size()), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(_indices.size() * sizeof(GLuint)), _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>((8 * sizeof(float)) + sizeof(int)));
    glEnableVertexAttribArray(4);
}

void Inventory::bind() const {
    glBindVertexArray(_vaoId);
}

void Inventory::render() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, static_cast<void *>(nullptr));
}

void Inventory::cleanup() const {
    deleteBuffers();
}

glm::mat4 Inventory::getModelMatrix() const {
    const auto radians = static_cast<float>(glm::radians(glfwGetTime() * 50.0));
    constexpr glm::vec3 position{40.0f, 40.0f, 0.0f};

    glm::mat4 matrix{1.0f};

    matrix = glm::translate(matrix, position);
    matrix = glm::translate(matrix, glm::vec3{_blockSize * 0.5f});
    matrix = glm::rotate(matrix, static_cast<float>(glm::radians(15.0)), glm::vec3{1.0f, 0.0f, 0.0f});
    matrix = glm::rotate(matrix, radians, glm::vec3{0.0f, 1.0f, 0.0f});
    matrix = glm::translate(matrix, glm::vec3{-_blockSize * 0.5f});
    matrix = glm::scale(matrix, glm::vec3{_blockSize});

    return matrix;
}

glm::mat4 Inventory::getViewMatrix() {
    return glm::mat4{1.0f};
}

glm::mat4 Inventory::getProjectionMatrix(const Window *window) {
    const auto width = window->getWidth();
    const auto height = window->getHeight();

    return glm::ortho(
        0.0f,
        static_cast<float>(width),
        0.0f,
        static_cast<float>(height),
        -300.0f,
        300.0f
    );
}

const Light &Inventory::getLight() const {
    return _light;
}


