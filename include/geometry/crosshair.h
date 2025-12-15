#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "render/vertexData.h"
#include "render/window.h"

class Crosshair {
public:
    GLuint _vboId{};
    GLuint _vaoId{};

    static constexpr std::array<VertexData, 6> VERTICES = {{
        {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}}
    }};

    Crosshair();

    static glm::mat4 localToWorldMatrix(const Window* window, const int crosshairSize = 1) {
        auto scaleFactor = static_cast<float>(window->getHeight()) * 0.00001;
        scaleFactor *= crosshairSize;

        return glm::scale(glm::mat4{1.0f}, glm::vec3(scaleFactor / window->getAspectRatio(), scaleFactor, 0.01f));
    }
};

