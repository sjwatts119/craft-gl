#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "core/blockType.h"
#include "geometry/AABB.h"
#include "utility/coordinate.h"
#include "material/blockTextureList.h"
#include "render/buffer/blockData.h"
#include "render/buffer/vertexData.h"
#include "utility/blockFace.h"

class Block {
private:
    BlockType _type;
    bool _highlighted;
    bool _destructible;
    float _slipperiness;

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

    static constexpr std::array<GLuint, 6> TOP_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> BOTTOM_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> BACK_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> FRONT_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> LEFT_INDICES = {0, 1, 2, 2, 3, 0};

    static constexpr std::array<GLuint, 6> RIGHT_INDICES = {0, 1, 2, 2, 3, 0};

public:
    explicit Block(BlockType type);

    static bool transparentFromType(const BlockType &type);

    static bool destructibleFromType(const BlockType &type);

    static float slipperinessFromType(const BlockType &type);

    static BlockTextureId textureIdFromTypeAndFace(const BlockType &type, const BlockFace &face);

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

    [[nodiscard]] std::array<BlockData, 24> getBufferVertices() const {
        std::array<BlockData, 24> vertices{};

        auto i = 0;

        for (const auto&[position, normal, texCoords] : getLeftVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_LEFT),
                false,
            };
        }

        for (const auto&[position, normal, texCoords] : getRightVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_RIGHT),
                false,
            };
        }

        for (const auto&[position, normal, texCoords] : getTopVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_TOP),
                false,
            };
        }

        for (const auto&[position, normal, texCoords] : getBottomVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_BOTTOM),
                false,
            };
        }

        for (const auto&[position, normal, texCoords] : getBackVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_BACK),
                false,
            };
        }

        for (const auto&[position, normal, texCoords] : getFrontVertices()) {
            vertices[i++] = BlockData {
                position.x, position.y, position.z,
                normal.x, normal.y, normal.z,
                texCoords.x, texCoords.y,
                getTextureId(FACE_FRONT),
                false,
            };
        }

        return vertices;
    }

    static std::array<GLuint, 36> getBufferIndices() {
        std::array<GLuint, 36> indices{};
        auto offset = 0;

        for (const auto index : getLeftIndices()) {
            indices[offset++] = index;
        }
        for (const auto index : getRightIndices()) {
            indices[offset++] = index + 4;
        }
        for (const auto index : getTopIndices()) {
            indices[offset++] = index + 8;
        }
        for (const auto index : getBottomIndices()) {
            indices[offset++] = index + 12;
        }
        for (const auto index : getBackIndices()) {
            indices[offset++] = index + 16;
        }
        for (const auto index : getFrontIndices()) {
            indices[offset++] = index + 20;
        }

        return indices;
    }

    [[nodiscard]] BlockType getType() const;

    void setType(const BlockType &type);

    [[nodiscard]] float getSlipperinessFactor() const;

    void setSlipperinessFactor(float slipperiness);

    [[nodiscard]] bool getDestructible() const;

    void setDestructible(bool destructible);

    [[nodiscard]] BlockTextureId getTextureId(BlockFace face) const;
};
