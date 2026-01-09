#pragma once

class Renderable {
public:
    virtual ~Renderable() = default;

    virtual void bind() = 0;

    virtual void render() = 0;
};