#include "renderable/debug.h"

Debug::Debug() {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
    glGenBuffers(1, &_eboId);
}

void Debug::update(const Player* player, const World* world) {
    _vertices.clear();
    _indices.clear();

    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        const auto& chunkAABB = chunk->_boundingBox;
        addAABB(chunkAABB);
    }

    addAABB(player->getBoundingBox());
}

/**
 * Add vertices and indices for an AABB
 */
void Debug::addAABB(const AABB& aabb) {
    const auto indexCount = _vertices.size();

    _vertices.push_back({aabb.minX, aabb.minY, aabb.minZ});
    _vertices.push_back({aabb.maxX, aabb.minY, aabb.minZ});
    _vertices.push_back({aabb.maxX, aabb.maxY, aabb.minZ});
    _vertices.push_back({aabb.minX, aabb.maxY, aabb.minZ});
    _vertices.push_back({aabb.minX, aabb.minY, aabb.maxZ});
    _vertices.push_back({aabb.maxX, aabb.minY, aabb.maxZ});
    _vertices.push_back({aabb.maxX, aabb.maxY, aabb.maxZ});
    _vertices.push_back({aabb.minX, aabb.maxY, aabb.maxZ});

    const std::array<int, 24> edges = {
        0,1, 1,2, 2,3, 3,0, // front face
        4,5, 5,6, 6,7, 7,4, // back face
        0,4, 1,5, 2,6, 3,7  // connecting edges
    };

    for (const auto& edge : edges) {
        _indices.push_back(indexCount + edge);
    }
}

void Debug::bind() {
    glBindVertexArray(_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(AABBData::size() * _vertices.size()), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(_indices.size() * sizeof(GLuint)), _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(AABBData::size()), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
}

void Debug::render() {
    glDrawElements(GL_LINES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
}


