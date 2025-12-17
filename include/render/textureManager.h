#pragma once

#include "texture.h"
#include "textureArray.h"

class TextureManager {
public:
    /**
     * ORDER IS SENSITIVE, must match enum definition "BlockType" in block.h (from 0)
     */
    TextureArray _blockTextures{
        {
            "../asset/texture/block/goodVibes/grass.png",
            "../asset/texture/block/goodVibes/dirt.png",
            "../asset/texture/block/goodVibes/stone.png",
            "../asset/texture/block/goodVibes/bedrock.png",
            "../asset/texture/block/goodVibes/diamond_block.png",
        },
        GL_RGBA,
        true
    };
};
