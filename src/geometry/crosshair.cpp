#include "geometry/crosshair.h"

Crosshair::Crosshair() {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
}
