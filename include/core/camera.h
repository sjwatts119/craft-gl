#pragma once

#include "utility/direction.h"
#include "glm/fwd.hpp"
#include "render/window.h"
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "geometry/AABB.h"

class Camera {

public:
    /** Euler Angles **/
    float _pitch = 0.0f;
    float _yaw = 0.0f;
    float _fov = 110.0f;

    /** Basis Vectors **/
    glm::vec3 _worldUp{0.0f, 1.0f, 0.0f};
    glm::vec3 _forward{0.0f, 0.0f, -1.0f};
    glm::vec3 _up = _worldUp;
    glm::vec3 _right{1.0f, 0.0f, 0.0f};

    /** Position **/
    glm::vec3 _position;

    Camera(const glm::vec3 &position): _position(position) {
        updateBasisVectors();
    }

    void updateBasisVectors();

    void aim(float yawOffset, float pitchOffset);

    void zoom(float offset);

    void move(glm::vec3 newPosition);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(const int &width, const int &height) const;

    [[nodiscard]] glm::mat4 getProjectionMatrix(const Window *window) const;
};
