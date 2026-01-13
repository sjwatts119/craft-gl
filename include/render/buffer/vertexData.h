#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glad/glad.h>

struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static GLsizei size() {
        return
            sizeof(float) + sizeof(float) + sizeof(float) + // pos
            sizeof(float) + sizeof(float) + sizeof(float) + // normal
            sizeof(float) + sizeof(float); // tex coords
    }
};
