#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "core/coordinate.h"
#include "geometry/block.h"
#include "glm/vec3.hpp"
#include "core/coordinate.h"

class RenderManager {
private:
    std::pmr::unordered_map<Coordinate, Block, CoordinateHash> _blocks;

public:
    RenderManager();

    void addTestBlocks();

    void render();
};
