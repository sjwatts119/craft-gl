#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderable.h"
#include "render/buffer/vertexData.h"
#include "render/window.h"

class Crosshair final : public Renderable {
public:
    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> VERTICES = {
        {
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom-left
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // bottom-right
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // top-right
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    static constexpr std::array<GLuint, 6> INDICES = {0, 1, 2, 2, 3, 0};

    Crosshair();

    void upload() override;

    void bind() const override;

    void render() const override;

    void cleanup() const override;

    static glm::mat4 localToWorldMatrix(const Window *window, const int crosshairSize = 1) {
        auto scaleFactor = static_cast<float>(window->getHeight()) * 0.00001;
        scaleFactor *= crosshairSize;

        return glm::scale(glm::mat4{1.0f}, glm::vec3(scaleFactor / window->getAspectRatio(), scaleFactor, 0.01f));
    }
};
