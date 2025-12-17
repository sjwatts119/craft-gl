#pragma once

#include "texture.h"

class TextureManager {
public:
    Texture _bedrockTexture{"../asset/texture/block/bedrock.png", GL_RGBA, true};
    Texture _dirtTexture{"../asset/texture/block/dirt.png", GL_RGBA, true};
    Texture _stoneTexture{"../asset/texture/block/stone.png", GL_RGBA, true};
    Texture _grassTexture{"../asset/texture/block/grass.png", GL_RGBA, true};
};
