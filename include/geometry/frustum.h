#pragma once

#include <glm/vec4.hpp>

#include "AABB.h"
#include "core/camera.h"

struct Frustum {
private:
    /**
     * Gribb/Hartmann method
     */
    void buildFromMatrix(const glm::mat4 &combinedMatrix) {
        for (int i = 4; i--;) {
            left[i] = combinedMatrix[i][3] + combinedMatrix[i][0];
            right[i] = combinedMatrix[i][3] - combinedMatrix[i][0];
            bottom[i] = combinedMatrix[i][3] + combinedMatrix[i][1];
            top[i] = combinedMatrix[i][3] - combinedMatrix[i][1];
            near[i] = combinedMatrix[i][3] + combinedMatrix[i][2];
            far[i] = combinedMatrix[i][3] - combinedMatrix[i][2];
        }
    }

    void normalise() {
        left = glm::normalize(left);
        right = glm::normalize(right);
        bottom = glm::normalize(bottom);
        top = glm::normalize(top);
        near = glm::normalize(near);
        far = glm::normalize(far);
    }


public:
    glm::vec4 left;
    glm::vec4 right;
    glm::vec4 bottom;
    glm::vec4 top;
    glm::vec4 near;
    glm::vec4 far;

    Frustum(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
        buildFromMatrix(glm::mat4(projectionMatrix * viewMatrix));
    }

    Frustum(const Camera &camera, const int screenWidth, const int screenHeight) {
        buildFromMatrix(camera.getProjectionMatrix(screenWidth, screenHeight) * camera.getViewMatrix());
    }

    /**
     * Does the frustum intersect with the given AABB?
     *
     * Ignores far/near planes and is not 100% accurate
     */
    bool fastIntersects(const AABB &aabb) const {
        const glm::vec4* planes[] = {&left, &right, &bottom, &top};

        // for each plane, find the positive vertex and check if it's inside
        // this is a bit inaccurate, it might not cull some AABBs but oh well it's fast
        for (const auto* plane : planes) {
            const glm::vec3 positiveVertex = {
                (plane->x >= 0) ? aabb.maxX : aabb.minX,
                (plane->y >= 0) ? aabb.maxY : aabb.minY,
                (plane->z >= 0) ? aabb.maxZ : aabb.minZ
            };

            const auto inside = plane->x * positiveVertex.x + plane->y * positiveVertex.y + plane->z * positiveVertex.z + plane->w;

            if (inside < 0) {
                return false;
            }
        }

        return true;
    }
};
