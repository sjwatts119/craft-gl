#pragma once
#include <string>
#include <glm/glm.hpp>

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

    [[nodiscard]] bool isInBounds() const {
        if (x < 0 || x > 15) {
            return false;
        }

        if (y < 0 || y > 15) {
            return false;
        }

        if (z < 0 || z > 15) {
            return false;
        }

        return true;
    }

    bool operator==(const Coordinate& coordinate) const {
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

    [[nodiscard]] glm::vec3 toVec3() const {
        return glm::vec3{x, y, z};
    }
};

/**
 * unordered_map needs a way to hash the object used as a key, so we use this struct.
 *
 * @see https://www.codegenes.net/blog/c-unordered-map-using-a-custom-class-type-as-the-key/
 */
struct CoordinateHash {
    size_t operator()(const Coordinate& coordinate) const {
        const size_t x = std::hash<int>{}(coordinate.x);
        const size_t y = std::hash<int>{}(coordinate.y);
        const size_t z = std::hash<int>{}(coordinate.z);

        return x ^ (y << 1) ^ (z << 1);
    }
};