#pragma once
#include "render/shader.h"

class ShaderManager {
public:
    Shader _blockShader{"../shader/block/vertex.glsl", "../shader/block/fragment.glsl"};
    Shader _crosshairShader{"../shader/crosshair/vertex.glsl", "../shader/crosshair/fragment.glsl"};
};
