#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "AABB.h"
#include "core/coordinate.h"
#include "material/blockTextureList.h"
#include "render/buffer/vertexData.h"
#include "geometry/blockFace.h"

enum BlockType: int {
    ERROR = -2,
    AIR = -1,

    GRASS = 0,
    DIRT = 1,
    STONE = 2,
    BEDROCK = 3,
    DIAMOND_BLOCK = 4
};

class Block {
private:
    BlockType _type;
    bool _highlighted;
    bool _destructible; // TODO : implement indestructible blocks

    static constexpr std::array<VertexData, 6> TOP_VERTICES = {
        {
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
        }
    };

    static constexpr std::array<VertexData, 6> BOTTOM_VERTICES = {
        {
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
        }
    };

    static constexpr std::array<VertexData, 6> BACK_VERTICES = {
        {
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}
        }
    };

    static constexpr std::array<VertexData, 6> FRONT_VERTICES = {
        {
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}
        }
    };

    static constexpr std::array<VertexData, 6> LEFT_VERTICES = {
        {
            {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        }
    };

    static constexpr std::array<VertexData, 6> RIGHT_VERTICES = {
        {
            {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        }
    };

public:
    Block(const BlockType type = AIR) : _type(type), _highlighted(false) {
    };

    [[nodiscard]] BlockType getType() const;

    void setType(const BlockType &type);

    static constexpr const std::array<VertexData, 6> &getTopVertices() {
        return TOP_VERTICES;
    }

    static constexpr const std::array<VertexData, 6> &getBottomVertices() {
        return BOTTOM_VERTICES;
    }

    static constexpr const std::array<VertexData, 6> &getBackVertices() {
        return BACK_VERTICES;
    }

    static constexpr const std::array<VertexData, 6> &getFrontVertices() {
        return FRONT_VERTICES;
    }

    static constexpr const std::array<VertexData, 6> &getLeftVertices() {
        return LEFT_VERTICES;
    }

    static constexpr const std::array<VertexData, 6> &getRightVertices() {
        return RIGHT_VERTICES;
    }

    int getTextureId(const BlockFace face) const {
        switch (_type) {
            case AIR:
                return -1; // No texture
            case GRASS:
                switch (face) {
                    case FACE_TOP:
                        return static_cast<int>(BlockTextureId::GRASS_BLOCK_TOP);
                    case FACE_BOTTOM:
                        return static_cast<int>(BlockTextureId::DIRT);
                    default:
                        return static_cast<int>(BlockTextureId::GRASS_BLOCK_SIDE);
                }
            case DIRT:
                return static_cast<int>(BlockTextureId::DIRT);
            case STONE:
                return static_cast<int>(BlockTextureId::STONE);
            case BEDROCK:
                return static_cast<int>(BlockTextureId::BEDROCK);
            case DIAMOND_BLOCK:
                return static_cast<int>(BlockTextureId::DIAMOND_BLOCK);
            default:
                return -2; // ERROR
        }
    }
};
