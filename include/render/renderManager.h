#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "shader/shader.h"
#include "shader/shaderManager.h"
#include "texture/textureManager.h"
#include "core/player.h"
#include "utility/coordinate.h"
#include "core/block/block.h"
#include "glm/vec3.hpp"
#include "utility/coordinate.h"
#include "render/renderable/debug.h"
#include "core/interface.h"
#include "glad/glad.h"
#include "render/shader/shader.h"
#include "render/buffer/crosshairData.h"
#include "core/world.h"
#include "core/interface.h"
#include "render/renderable/chunkMesh.h"

class RenderManager {
private:
    ShaderManager _shaderManager;
    TextureManager _textureManager;

    void renderBlocks(const Player *player, const Window *window, World *world) const;

    void renderInterface(const Window *window, Interface *interface) const;

    void renderDebug(const Player *player, const Window *window, Debug *debug) const;

public:
    explicit RenderManager(const Window *window);

    static void clear();

    void render(const Player *player, const Window *window, World *world, Interface *interface, Debug *debug) const;
};
