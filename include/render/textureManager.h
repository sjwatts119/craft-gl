#pragma once

#include "texture.h"

class TextureManager {
public:
    Texture _bedrockTexture{"../asset/texture/goodVibes/block/bedrock.png", GL_RGBA, true};
    Texture _dirtTexture{"../asset/texture/goodVibes/block/dirt.png", GL_RGBA, true};
    Texture _stoneTexture{"../asset/texture/goodVibes/block/stone.png", GL_RGBA, true};
    Texture _grassTexture{"../asset/texture/goodVibes/block/grass.png", GL_RGBA, true};
};
