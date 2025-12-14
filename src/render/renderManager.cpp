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

void RenderManager::addTestBlocks() {
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 100; y++) {
            for (int z = 0; z < 16; z++) {
                BlockType type;

                if (y == 0) {
                    type = BEDROCK;
                } else if (y < 46) {
                    type = STONE;
                } else if (y < 49) {
                    type = DIRT;
                } else if (y < 50) {
                    type = GRASS;
                } else {
                    type = AIR;
                }

                _blocks.insert({Coordinate {x, y, z}, Block{type}});
            }
        }
    }

    std::cout << "Added Test Blocks - Count: " << _blocks.size() << std::endl;
};

void RenderManager::renderBlocks(const Camera* camera, const Window* window) const {
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

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 100; y++) {
            for (int z = 0; z < 16; z++) {
                Coordinate coordinate {x, y, z};

                const BlockType type = _blocks.find(coordinate)->second.getType();

                if (type == AIR) {
                    continue;
                }

                glm::mat4 modelMatrix = glm::translate(glm::mat4{1.0f}, coordinate.toVec3());

                _blockShader.setMat4("uModelMatrix", modelMatrix);

                _blockShader.setInt("uBlockType", type);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glBindVertexArray(0);
}

void RenderManager::render(const Camera* camera, const Window* window) {
    renderBlocks(camera, window);

    // Foreach of stored types, render.


}

