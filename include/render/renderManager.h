#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "shader.h"
#include "core/camera.h"
#include "core/coordinate.h"
#include "geometry/block.h"
#include "glm/vec3.hpp"
#include "core/coordinate.h"
#include "glad/glad.h"
#include "render/shader.h"

class RenderManager {
private:
    Shader _blockShader{"../shader/block/vertex.glsl", "../shader/block/fragment.glsl"};

    unsigned int _blockVboId;
    unsigned int _blockVaoId;

    std::pmr::unordered_map<Coordinate, Block, CoordinateHash> _blocks;

    void renderBlocks(const Camera* camera, const Window* window) const;

public:
    explicit RenderManager(const Window* window);

    static void clear();

    void addTestBlocks();

    void render(const Camera* camera, const Window* window) const;
};
