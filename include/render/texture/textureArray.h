#pragma once

#include <iostream>
#include <vector>

class TextureArray {
private:
    std::vector<std::string> _paths;
    int _width;
    int _height;
    int _channels;
    int _format;
    bool _flip = false;

    unsigned int _id;

public:
    TextureArray(std::vector<std::string> paths, int format, bool flip = false);

    void setup();

    void use(unsigned int unit) const;

    [[nodiscard]] unsigned int getId() const;
};
