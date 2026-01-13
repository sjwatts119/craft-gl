#pragma once

#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderable.h"
#include "core/blockType.h"
#include "material/light.h"
#include "render/window.h"
#include "render/buffer/blockData.h"

class Inventory final : public Renderable {
private:
    BlockType _selectedBlockType = BlockType::DIAMOND_BLOCK;

    std::array<BlockData, 24> _vertices;
    std::array<GLuint, 36> _indices;

    const float _blockSize = 75.0f;
    Light _light{
        glm::vec3{_blockSize, _blockSize * 2.0f, _blockSize * 2.0f},
        glm::vec3{0.7f, 0.7f, 0.7f},
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.6f, 0.6f, 0.6f)
    };
public:
    Inventory();

    [[nodiscard]] BlockType getSelectedBlockType() const;

    void selectBlockType(BlockType type);

    void selectNextBlockType();

    void selectPreviousBlockType();

    void regenerateMesh();

    void upload() override;

    void bind() const override;

    void render() const override;

    void cleanup() const override;

    [[nodiscard]] glm::mat4 getModelMatrix() const;

    static glm::mat4 getViewMatrix() ;

    static glm::mat4 getProjectionMatrix(const Window *window);

    [[nodiscard]] const Light &getLight() const;
};
