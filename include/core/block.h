#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "core/blockType.h"
#include "geometry/AABB.h"
#include "utility/coordinate.h"
#include "material/blockTextureList.h"
#include "render/buffer/vertexData.h"
#include "utility/blockFace.h"

class Block {
private:
    BlockType _type;
    bool _highlighted;
    bool _destructible;
    float _slipperiness;

    static constexpr std::array<VertexData, 24> ALL_VERTICES = {{
        // Top face
        {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // top-left

        // Bottom face
        {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
        {{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
        {{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
        {{0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // top-left

        // Back face
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  // bottom-left
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},  // bottom-right
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},  // top-right
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // top-left

        // Front face
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // bottom-left
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // bottom-right
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // top-right
        {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // top-left

        // Left face
        {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
        {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
        {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
        {{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // top-left

        // Right face
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // top-left
    }};

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> TOP_VERTICES = {
        {
            {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
            {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
            {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
            {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> BOTTOM_VERTICES = {
        {
            {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
            {{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
            {{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
            {{0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> BACK_VERTICES = {
        {
            {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  // bottom-left
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},  // bottom-right
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},  // top-right
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> FRONT_VERTICES = {
        {
            {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // bottom-left
            {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // bottom-right
            {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // top-right
            {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> LEFT_VERTICES = {
        {
            {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
            {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
            {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
            {{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    /**
     * Counter-clockwise winding
     */
    static constexpr std::array<VertexData, 4> RIGHT_VERTICES = {
        {
            {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
            {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // top-right
            {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // top-left
        }
    };

    static constexpr std::array<GLuint, 36> ALL_INDICES = {
        // Top face
        0, 1, 2, 2, 3, 0,
        // Bottom face
        4, 5, 6, 6, 7, 4,
        // Back face
        8, 9, 10, 10, 11, 8,
        // Front face
        12, 13, 14, 14, 15, 12,
        // Left face
        16, 17, 18, 18, 19, 16,
        // Right face
        20, 21, 22, 22, 23, 20
    };

    static constexpr std::array<GLuint, 6> TOP_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> BOTTOM_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> BACK_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> FRONT_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> LEFT_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> RIGHT_INDICES = {0, 1, 2, 2, 3, 0};

public:
    explicit Block(BlockType type);

    static bool destructibleFromType(BlockType type);

    static float slipperinessFromType(BlockType type);

    static BlockTextureId textureIdFromTypeAndFace(BlockType type, BlockFace face);

    static constexpr const std::array<VertexData, 24> &getAllVertices() {
        return ALL_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getTopVertices() {
        return TOP_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getBottomVertices() {
        return BOTTOM_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getBackVertices() {
        return BACK_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getFrontVertices() {
        return FRONT_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getLeftVertices() {
        return LEFT_VERTICES;
    }

    static constexpr const std::array<VertexData, 4> &getRightVertices() {
        return RIGHT_VERTICES;
    }

    static constexpr const std::array<GLuint, 36> &getAllIndices() {
        return ALL_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getTopIndices() {
        return TOP_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getBottomIndices() {
        return BOTTOM_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getBackIndices() {
        return BACK_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getFrontIndices() {
        return FRONT_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getLeftIndices() {
        return LEFT_INDICES;
    }

    static constexpr const std::array<GLuint, 6> &getRightIndices() {
        return RIGHT_INDICES;
    }

    [[nodiscard]] BlockType getType() const;

    void setType(const BlockType &type);

    [[nodiscard]] float getSlipperinessFactor() const;

    void setSlipperinessFactor(float slipperiness);

    [[nodiscard]] bool getDestructible() const;

    void setDestructible(bool destructible);

    [[nodiscard]] BlockTextureId getTextureId(BlockFace face) const;
};
