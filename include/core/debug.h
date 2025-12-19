#pragma once

#include <glad/glad.h>
#include "core/player.h"
#include "core/world.h"
#include "render/buffer/aabbData.h"

class Debug {
private:
    GLuint _vboId;
    GLuint _vaoId;
    GLuint _eboId;

    std::vector<AABBData> _vertices;
    std::vector<GLuint> _indices;

private:
    void addAABB(const AABB& aabb);
public:
    Debug();

    void update(const Player* player, const World* world);

    void bind() const;

    void render() const;
};
