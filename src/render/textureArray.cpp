#include "render/texture/textureArray.h"

#include <stb/stb_image.h>

#include <utility>

TextureArray::TextureArray(std::vector<std::string> paths, const int format, const bool flip)
    : _paths(std::move(paths)), _format(format), _flip(flip) {
    setup();
}

void TextureArray::setup() {
    // Generate 1 texture and store the ID in textureId
    glGenTextures(1, &_id);

    // Bind the texture to the current OpenGL context
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);

    // Set the wrapping and filtering parameters for this texture
    // Wrap in x and y direction (S and T) by repeating
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Downsize textures using our mipmaps with linear interpolation
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // Upsize textures using linear interpolation
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Flip the image vertically on load to match OpenGL's coordinate system
    stbi_set_flip_vertically_on_load(_flip);

    // Load the first texture from the file path to get the image dimensions
    unsigned char *firstImageData = stbi_load(_paths[0].c_str(), &_width, &_height, &_channels, 0);

    std::cout << "Loaded texture array layer " << 0 << " from path: " << _paths[0] << std::endl;

    if (!firstImageData) {
        std::cerr << "Failed to load first texture at path: " << _paths[0] << std::endl;

        return;
    }

    // Generate the texture array storage with the dimensions of the first image and depth of total path count
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, _format, _width, _height, static_cast<GLsizei>(_paths.size()), 0, _format,
                 GL_UNSIGNED_BYTE, nullptr);

    // Upload the first image
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 1,
                    _format, GL_UNSIGNED_BYTE, firstImageData);

    // free the image memory for the first image
    stbi_image_free(firstImageData);

    // iterate over the rest of the paths, read them in and upload them (starting from layer 1)
    for (size_t i = 1; i < _paths.size(); i++) {
        unsigned char *imageData = stbi_load(_paths[i].c_str(), &_width, &_height, &_channels, 0);

        std::cout << "Loaded texture array layer " << i << " from path: " << _paths[i] << std::endl;

        if (!imageData) {
            std::cerr << "Failed to load texture at path: " << _paths[i] << std::endl;

            return;
        }

        // upload the image to the texture array at layer i
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, static_cast<GLint>(i), static_cast<GLsizei>(_width),
                        static_cast<GLsizei>(_height), 1, _format, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
    }

    // generate mipmaps for each texture in the array
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void TextureArray::use(const unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
}

unsigned int TextureArray::getId() const {
    return _id;
}
