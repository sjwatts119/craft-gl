#include "geometry/AABB.h"

AABB AABB::forBlock(const Coordinate &coord) {
    return {
            {
                static_cast<float>(coord.x),
                static_cast<float>(coord.y),
                static_cast<float>(coord.z)
            },
            {
                static_cast<float>(coord.x) + 1.0f,
                static_cast<float>(coord.y) + 1.0f,
                static_cast<float>(coord.z) + 1.0f
            }
    };
}

AABB AABB::forChunk(const Coordinate &chunkCoord) {
    const auto worldCoord = chunkCoord.toWorldFromChunk();

    return {
            {
                static_cast<float>(worldCoord.x),
                static_cast<float>(worldCoord.y),
                static_cast<float>(worldCoord.z)
            },
            {
                static_cast<float>(worldCoord.x) + CHUNK_SIZE,
                static_cast<float>(worldCoord.y) + CHUNK_SIZE,
                static_cast<float>(worldCoord.z) + CHUNK_SIZE
            }
    };
}

AABB AABB::forPlayer(const glm::vec3 &position, const float playerWidth, const float playerHeight) {
    return {
            {
                position.x - (playerWidth / 2),
                position.y,
                position.z - (playerWidth / 2)
            },
            {
                position.x + (playerWidth / 2),
                position.y + playerHeight,
                position.z + (playerWidth / 2)
            }
    };
}

void AABB::expandTo(const glm::vec3 &point) {
    minX = std::min(minX, point.x);
    minY = std::min(minY, point.y);
    minZ = std::min(minZ, point.z);
    maxX = std::max(maxX, point.x);
    maxY = std::max(maxY, point.y);
    maxZ = std::max(maxZ, point.z);
}

void AABB::include(AABB &other) {
    expandTo({other.minX, other.minY, other.minZ});
    expandTo({other.maxX, other.maxY, other.maxZ});
}

bool AABB::intersectsX(const AABB &aabb) const {
    return (minX <= aabb.maxX && maxX >= aabb.minX);
}

bool AABB::intersectsY(const AABB &aabb) const {
    return (minY <= aabb.maxY && maxY >= aabb.minY);
}

bool AABB::intersectsZ(const AABB &aabb) const {
    return (minZ <= aabb.maxZ && maxZ >= aabb.minZ);
}

bool AABB::intersects(const AABB &aabb) const {
    return intersectsX(aabb) && intersectsY(aabb) && intersectsZ(aabb);
}

/**
 * Massive thank you to Andre Blunt for a brilliant article on clipping against AABBs.
 *
 * https://medium.com/@andrebluntindie/3d-aabb-collision-detection-and-resolution-for-voxel-games-5fcbfdb8cdb4
 */

/**
 * Clips deltaX to prevent intersection with another AABB.
 */
float AABB::clipX(const AABB &against, float deltaX) const {
    // Do we even actually intersect on the other axes?
    if (!intersectsY(against) || !intersectsZ(against)) {
        return deltaX;
    }

    // Moving right
    if (deltaX > 0 && maxX <= against.minX) {
        if (const auto clip = against.minX - maxX - EPSILON; deltaX > clip) {
            deltaX = clip;
        }
    }

    // Moving left
    if (deltaX < 0 && minX >= against.maxX) {
        if (const auto clip = against.maxX - minX + EPSILON; deltaX < clip) {
            deltaX = clip;
        }
    }

    return deltaX;
}

float AABB::clipY(const AABB &against, float deltaY) const {
    // Do we even actually intersect on the other axes?
    if (!intersectsX(against) || !intersectsZ(against)) {
        return deltaY;
    }

    // Moving up
    if (deltaY > 0 && maxY <= against.minY) {
        if (const auto clip = against.minY - maxY - EPSILON; deltaY > clip) {
            deltaY = clip;
        }

    }

    // Moving down
    if (deltaY < 0 && minY >= against.maxY) {
        if (const auto clip = against.maxY - minY + EPSILON; deltaY < clip) {
            deltaY = clip;
        }
    }

    return deltaY;
}

float AABB::clipZ(const AABB &against, float deltaZ) const {
    if (!intersectsX(against) || !intersectsY(against)) {
        return deltaZ;
    }

    // Moving backwards
    if (deltaZ > 0 && maxZ <= against.minZ) {
        if (const auto clip = against.minZ - maxZ - EPSILON; deltaZ > clip) {
            deltaZ = clip;
        }
    }

    // Moving forwards
    if (deltaZ < 0 && minZ >= against.maxZ) {
        if (const auto clip = against.maxZ - minZ + EPSILON; deltaZ < clip) {
            deltaZ = clip;
        }
    }

    return deltaZ;
}


