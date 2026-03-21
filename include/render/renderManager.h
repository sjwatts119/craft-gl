#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>

#include "shader/shader.h"
#include "shader/shaderManager.h"
#include "texture/textureManager.h"
#include "core/player.h"
#include "utility/coordinate.h"
#include "core/block.h"
#include "glm/vec3.hpp"
#include "utility/coordinate.h"
#include "render/renderable/debug.h"
#include "glad/glad.h"
#include "render/shader/shader.h"
#include "render/buffer/crosshairData.h"
#include "core/world.h"
#include "render/renderable/chunkMesh.h"
#include "render/renderable/crosshair.h"

class RenderManager {
private:
    ShaderManager _shaderManager;
    TextureManager _textureManager;

    void renderBlocks() const;

    void renderCrosshair() const;

    void renderInventory() const;

    void renderInterface() const;

    void renderDebug() const;
public:
    RenderManager();

    void clear() const;

    void render() const;
};
