#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "core/camera.h"
#include "core/coordinate.h"
#include "geometry/block.h"
#include "glm/vec3.hpp"
#include "core/coordinate.h"
#include "glad/glad.h"

class RenderManager {
private:
    std::pmr::unordered_map<Coordinate, Block, CoordinateHash> _blocks;

    static void renderBlocks(const Camera* camera, const Window* window);

public:
    explicit RenderManager(const Window* window);

    static void clear();

    void addTestBlocks();

    static void render(const Camera* camera, const Window* window);
};
