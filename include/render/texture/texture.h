#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>


class Texture {
private:
    std::string _path;
    int _width;
    int _height;
    int _channels;
    int _format;
    bool _flip = false;

    unsigned int _id;

public:
    Texture(std::string path, int format, bool flip = false);

    void setup();

    void use(unsigned int unit) const;

    [[nodiscard]] unsigned int getId() const;
};
