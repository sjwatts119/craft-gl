#pragma once
#include <string>

struct Coordinate {
    int x;
    int y;
    int z;

    bool operator==(const Coordinate& coordinate) const {
        return coordinate.x == x && coordinate.y && y && coordinate.z == z;
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