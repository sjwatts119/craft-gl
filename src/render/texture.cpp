#include "render/texture/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <utility>

Texture::Texture(std::string path, const int format, const bool flip)
    : _path(std::move(path)), _format(format), _flip(flip) {
    setup();
}

void Texture::setup() {
    // Flip the image vertically on load to match OpenGL's coordinate system
    stbi_set_flip_vertically_on_load(_flip);

    // Load the texture data from file
    unsigned char *textureData = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);

    stbi_image_free(textureData);

    // Generate 1 texture and store the ID in textureId
    glGenTextures(1, &_id);

    // Bind the texture to the current OpenGL context
    glBindTexture(GL_TEXTURE_2D, _id);

    // Set the wrapping and filtering parameters for this texture
    // Wrap in x and y direction (S and T) by repeating
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Downsize textures using our mipmaps with linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // Upsize textures using linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload the texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (!textureData) {
        std::cerr << "Failed to load texture: {" << _path << "}" << std::endl;

        return;
    }
}

void Texture::use(const unsigned int unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

unsigned int Texture::getId() const {
    return _id;
}
