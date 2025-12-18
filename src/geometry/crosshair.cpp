#include "geometry/crosshair.h"

#include "render/buffer/crosshairData.h"

Crosshair::Crosshair() {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
    glGenBuffers(1, &_eboId);
}

void Crosshair::bind() const {
    glBindVertexArray(_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(CrosshairData::size() * VERTICES.size()), VERTICES.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(INDICES)), INDICES.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Crosshair::render() {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(INDICES.size()), GL_UNSIGNED_INT, static_cast<void *>(nullptr));
}