#include <render/renderManager.h>

RenderManager::RenderManager(const Window* window) {
    glViewport(0, 0, window->getWidth(), window->getHeight());

    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks(const Player* player, const Window* window, const World* world) {
    _shaderManager._blockShader.use();

    _textureManager._bedrockTexture.use(GL_TEXTURE0);
    _textureManager._dirtTexture.use(GL_TEXTURE1);
    _textureManager._stoneTexture.use(GL_TEXTURE2);
    _textureManager._grassTexture.use(GL_TEXTURE3);

    _shaderManager._blockShader.setInt("uBedrockTexture", 0);
    _shaderManager._blockShader.setInt("uDirtTexture", 1);
    _shaderManager._blockShader.setInt("uStoneTexture", 2);
    _shaderManager._blockShader.setInt("uGrassTexture", 3);

    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        glBindVertexArray(chunk->_mesh->_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, chunk->_mesh->_vboId);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(BlockData::size() * chunk->_mesh->_vertices.size()), chunk->_mesh->_vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribIPointer(3, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glVertexAttribIPointer(4, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>((8 * sizeof(float)) + sizeof(int)));
        glEnableVertexAttribArray(4);

        _shaderManager._blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());
        _shaderManager._blockShader.setMat4("uViewMatrix", player->getViewMatrix());
        _shaderManager._blockShader.setMat4("uProjectionMatrix", player->getProjectionMatrix(window));

        _shaderManager._blockShader.setLight("uSun" , world->getSun());


        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(chunk->_mesh->_vertices.size()));
    }

    glBindVertexArray(0);
}

void RenderManager::renderInterface(const Window *window, const World *world, const Interface *interface) {
    glBindVertexArray(interface->_crosshair._vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, interface->_crosshair._vboId);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(CrosshairData::size() * Crosshair::VERTICES.size()), Crosshair::VERTICES.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(CrosshairData::size()), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    _shaderManager._crosshairShader.use();

    _shaderManager._crosshairShader.setMat4("uModelMatrix", Crosshair::localToWorldMatrix(window));

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Crosshair::VERTICES.size()));

    glBindVertexArray(0);
}

void RenderManager::render(const Player* player, const Window* window, const World* world, const Interface* interface) {
    renderBlocks(player, window, world);

    renderInterface(window, world, interface);
}
