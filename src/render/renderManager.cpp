#include <render/renderManager.h>

#include "render/shader.h"

RenderManager::RenderManager(const Window* window) {
    glViewport(0, 0, window->getWidth(), window->getHeight());

    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::addTestBlocks() {
    Block block;

    // 16x1x16 blocks
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            _blocks.insert({Coordinate {x, 1, z}, block});
        }
    }

    std::cout << "Added Test Blocks - Count: " << _blocks.size() << std::endl;
};

void RenderManager::renderBlocks(const Camera* camera, const Window* window) {
    // Set up VBO
    unsigned int vboId;
    glGenBuffers(1, &vboId);

    // Set up VAO
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Get cube vertices
    const auto vertices = Block::getVertices();

    // Bind the VBO to the context
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // Upload the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Set the layout/stride information on the bound VAO for this data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    const Shader shader("../shader/block/vertex.glsl", "../shader/block/fragment.glsl");

    shader.use();

    shader.setMat4("uViewMatrix", camera->getViewMatrix());
    shader.setMat4("uProjectionMatrix", camera->getProjectionMatrix(window));

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            Coordinate coordinate {x, 1, z};

            glm::mat4 modelMatrix = glm::translate(glm::mat4{1.0f}, coordinate.toVec3());

            shader.setMat4("uModelMatrix", modelMatrix);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glBindVertexArray(0);
}

void RenderManager::render(const Camera* camera, const Window* window) {
    renderBlocks(camera, window);

    // Foreach of stored types, render.


}

