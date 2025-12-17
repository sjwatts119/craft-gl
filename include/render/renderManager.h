#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "shader.h"
#include "shaderManager.h"
#include "textureManager.h"
#include "core/player.h"
#include "core/coordinate.h"
#include "geometry/block.h"
#include "glm/vec3.hpp"
#include "core/coordinate.h"
#include "core/interface.h"
#include "glad/glad.h"
#include "render/shader.h"
#include "render/buffer/crosshairData.h"
#include "core/world.h"
#include "core/interface.h"
#include "geometry/chunkMesh.h"

class RenderManager {
private:
    ShaderManager _shaderManager;
    TextureManager _textureManager;

    std::pmr::unordered_map<Coordinate, Block, CoordinateHash> _blocks;

    void renderBlocks(const Player* player, const Window* window, const World* world) const;

    void renderInterface(const Window *window, const World *world, const Interface *interface) const;

public:
    explicit RenderManager(const Window* window);

    static void clear();

    void render(const Player* player, const Window* window, const World* world, const Interface* interface) const;
};
