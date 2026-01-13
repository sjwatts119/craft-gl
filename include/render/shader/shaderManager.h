#pragma once
#include "render/shader/shader.h"

class ShaderManager {
public:
    Shader _blockShader{"../shader/block/vertex.glsl", "../shader/block/fragment.glsl"};
    Shader _crosshairShader{"../shader/crosshair/vertex.glsl", "../shader/crosshair/fragment.glsl"};
    Shader _debugShader{"../shader/debug/vertex.glsl", "../shader/debug/fragment.glsl"};
    Shader _inventoryShader{"../shader/inventory/vertex.glsl", "../shader/inventory/fragment.glsl"};
};
