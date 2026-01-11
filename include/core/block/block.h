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
protected:
    BlockType _type;
    bool _highlighted;
    bool _destructible;
    float _slipperiness;

    Block(const BlockType type, const bool destructible, const float slipperiness)
        : _type(type), _highlighted(false), _destructible(destructible), _slipperiness(slipperiness) {}

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
    virtual ~Block() = default;

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

    [[nodiscard]] BlockType getType() const;

    void setType(const BlockType &type);

    [[nodiscard]] float getSlipperinessFactor() const;

    void setSlipperinessFactor(float slipperiness);

    [[nodiscard]] bool getDestructible() const;

    void setDestructible(bool destructible);

    [[nodiscard]]

    virtual BlockTextureId getTextureId(BlockFace face) = 0;
};
