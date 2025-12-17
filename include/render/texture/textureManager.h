#pragma once

#include "texture.h"
#include "textureArray.h"
#include "material/blockTextureList.h"

class TextureManager {
public:
    TextureArray _blockTextures{
        BlockTextureList::allPaths(),
        GL_RGBA,
        true
    };
};
