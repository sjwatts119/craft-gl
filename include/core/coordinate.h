#pragma once
#include <string>
#include <glm/glm.hpp>

#include "staticData.h"
#include "geometry/block.h"
#include "geometry/blockFace.h"

struct Coordinate {
    int x;
    int y;
    int z;

    explicit Coordinate(const int _x, const int _y, const int _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    explicit Coordinate(const float _x, const float _y, const float _z) {
        x = static_cast<int>(_x);
        y = static_cast<int>(_y);
        z = static_cast<int>(_z);
    }

    explicit Coordinate(const glm::vec3 vec3) {
        x = static_cast<int>(vec3.x);
        y = static_cast<int>(vec3.y);
        z = static_cast<int>(vec3.z);
    }

    explicit Coordinate(const glm::ivec3 iVec3) {
        x = iVec3.x;
        y = iVec3.y;
        z = iVec3.z;
    }

    /**
     * Transform a coordinate from world coordinates to chunk coordinates (16, 0, 0) => (1, 0, 0)
     */
    [[nodiscard]] Coordinate toChunkFromWorld() const {
        return Coordinate{
            std::floor(static_cast<float>(x) / CHUNK_SIZE),
            std::floor(static_cast<float>(y) / CHUNK_SIZE),
            std::floor(static_cast<float>(z) / CHUNK_SIZE)
        };
    }

    /**
     * Transform a coordinate from chunk coordinates to local coordinates (1, 0, 0) => (16, 0, 0)
     */
    [[nodiscard]] Coordinate toLocalFromChunk() const {
        return Coordinate{
            x * CHUNK_SIZE,
            y * CHUNK_SIZE,
            z * CHUNK_SIZE
        };
    }

    /**
     * Transform a coordinate from local coordinates to global coordinates (1, 0, 0) + localOffset(5, 0, 0) => (21, 0, 0)
     */
    [[nodiscard]] Coordinate toGlobalFromChunk(const Coordinate &localOffset) const {
        return Coordinate{
            x * CHUNK_SIZE + localOffset.x,
            y * CHUNK_SIZE + localOffset.y,
            z * CHUNK_SIZE + localOffset.z
        };
    }

    [[nodiscard]] bool isInBounds() const {
        if (x < 0 || x >= CHUNK_SIZE) {
            return false;
        }

        if (y < 0 || y >= CHUNK_SIZE) {
            return false;
        }

        if (z < 0 || z >= CHUNK_SIZE) {
            return false;
        }

        return true;
    }

    bool operator==(const Coordinate &coordinate) const {
        return coordinate.x == x && coordinate.y == y && coordinate.z == z;
    }

    [[nodiscard]] Coordinate leftNeighbour() const {
        return Coordinate{x - 1, y, z};
    }

    [[nodiscard]] Coordinate rightNeighbour() const {
        return Coordinate{x + 1, y, z};
    }

    [[nodiscard]] Coordinate downNeighbour() const {
        return Coordinate{x, y - 1, z};
    }

    [[nodiscard]] Coordinate upNeighbour() const {
        return Coordinate{x, y + 1, z};
    }

    [[nodiscard]] Coordinate backNeighbour() const {
        return Coordinate{x, y, z - 1};
    }

    [[nodiscard]] Coordinate frontNeighbour() const {
        return Coordinate{x, y, z + 1};
    }

    [[nodiscard]] Coordinate moveTowards(const BlockFace face) const {
        switch (face) {
            case FACE_LEFT:
                return leftNeighbour();
            case FACE_RIGHT:
                return rightNeighbour();
            case FACE_BOTTOM:
                return downNeighbour();
            case FACE_TOP:
                return upNeighbour();
            case FACE_BACK:
                return backNeighbour();
            case FACE_FRONT:
                return frontNeighbour();
            default:
                return *this;
        }
    }

    [[nodiscard]] glm::vec3 toVec3() const {
        return glm::vec3{x, y, z};
    }

    [[nodiscard]] glm::ivec3 toIVec3() const {
        return glm::ivec3{static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)};
    }
};

/**
 * unordered_map needs a way to hash the object used as a key, so we use this struct.
 *
 * @see https://www.codegenes.net/blog/c-unordered-map-using-a-custom-class-type-as-the-key/
 */
struct CoordinateHash {
    size_t operator()(const Coordinate &coordinate) const {
        const size_t x = std::hash<int>{}(coordinate.x);
        const size_t y = std::hash<int>{}(coordinate.y);
        const size_t z = std::hash<int>{}(coordinate.z);

        return x ^ (y << 1) ^ (z << 1);
    }
};
