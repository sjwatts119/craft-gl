#pragma once

#include <glad/glad.h>

class Renderable {
protected:
    GLuint _vboId{};
    GLuint _vaoId{};
    GLuint _eboId{};

    void genBuffers() {
        glGenVertexArrays(1, &_vaoId);
        glGenBuffers(1, &_vboId);
        glGenBuffers(1, &_eboId);
    }

    void deleteBuffers() const {
        glDeleteBuffers(1, &_vboId);
        glDeleteBuffers(1, &_eboId);
        glDeleteVertexArrays(1, &_vaoId);
    }
public:
    virtual ~Renderable() = default;

    virtual void upload() = 0;

    virtual void bind() const = 0;

    virtual void render() const = 0;

    virtual void cleanup() const = 0;
};