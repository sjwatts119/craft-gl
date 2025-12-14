#include <render/renderManager.h>

RenderManager::RenderManager(const Window* window) {
    glViewport(0, 0, window->getWidth(), window->getHeight());

    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &_blockVboId);

    // Set up VAO
    glGenVertexArrays(1, &_blockVaoId);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks(const Camera* camera, const Window* window, const World* world) {
    // Bind the block VAO
    glBindVertexArray(_blockVaoId);

    // Get cube vertices
    const auto vertices = Block::getVertices();

    // Bind the VBO to the context
    glBindBuffer(GL_ARRAY_BUFFER, _blockVboId);

    // Upload the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Set the layout/stride information on the bound VAO for this data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    _blockShader.use();

    _blockShader.setMat4("uViewMatrix", camera->getViewMatrix());
    _blockShader.setMat4("uProjectionMatrix", camera->getProjectionMatrix(window));

    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        for (auto& [coordinate, block]: chunk._blocks) {
            const auto type = block.getType();

            if (type == AIR) {
                continue;
            }

            glm::mat4 localModelMatrix = glm::translate(glm::mat4{1.0f}, coordinate.toVec3());
            glm::mat4 modelMatrix = chunk.localToWorldMatrix() * localModelMatrix;

            _blockShader.setMat4("uModelMatrix", modelMatrix);

            _blockShader.setInt("uBlockType", type);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        }
    }


    glBindVertexArray(0);
}

void RenderManager::render(const Camera* camera, const Window* window, const World* world) {
    renderBlocks(camera, window, world);

    // Foreach of stored types, render.


}

