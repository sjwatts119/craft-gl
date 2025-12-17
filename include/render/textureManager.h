#pragma once

#include "texture.h"

class TextureManager {
public:
    Texture _bedrockTexture{"../asset/texture/block/goodVibes/bedrock.png", GL_RGBA, true};
    Texture _dirtTexture{"../asset/texture/block/goodVibes/dirt.png", GL_RGBA, true};
    Texture _stoneTexture{"../asset/texture/block/goodVibes/stone.png", GL_RGBA, true};
    Texture _grassTexture{"../asset/texture/block/goodVibes/grass.png", GL_RGBA, true};
    Texture _diamondBlockTexture{"../asset/texture/block/goodVibes/diamond_block.png", GL_RGBA, true};
};
