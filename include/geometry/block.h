#pragma once

#include <array>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "AABB.h"
#include "core/coordinate.h"

enum BlockType: int {
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    BEDROCK = 4,
    ERROR = 5,
};

class Block {
private:
    BlockType _type;

    static constexpr std::array<glm::vec3, 6> TOP_VERTICES = {{
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f}
    }};

    static constexpr std::array<glm::vec3, 6> BOTTOM_VERTICES = {{
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f}
    }};

    static constexpr std::array<glm::vec3, 6> BACK_VERTICES = {{
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f}
    }};

    static constexpr std::array<glm::vec3, 6> FRONT_VERTICES = {{
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f}
    }};

    static constexpr std::array<glm::vec3, 6> LEFT_VERTICES = {{
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}
    }};

    static constexpr std::array<glm::vec3, 6> RIGHT_VERTICES = {{
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}
    }};

public:
    explicit Block(const BlockType type = AIR): _type(type) {};

    [[nodiscard]] BlockType getType() const;

    void setType(const BlockType& type);

    static constexpr const std::array<glm::vec3, 6>& getTopVertices() {
        return TOP_VERTICES;
    }

    static constexpr const std::array<glm::vec3, 6>& getBottomVertices() {
        return BOTTOM_VERTICES;
    }

    static constexpr const std::array<glm::vec3, 6>& getBackVertices() {
        return BACK_VERTICES;
    }

    static constexpr const std::array<glm::vec3, 6>& getFrontVertices() {
        return FRONT_VERTICES;
    }

    static constexpr const std::array<glm::vec3, 6>& getLeftVertices() {
        return LEFT_VERTICES;
    }

    static constexpr const std::array<glm::vec3, 6>& getRightVertices() {
        return RIGHT_VERTICES;
    }
};