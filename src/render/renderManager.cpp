#include <render/renderManager.h>

#include "geometry/chunkMesh.h"

RenderManager::RenderManager(const Window* window) {
    glViewport(0, 0, window->getWidth(), window->getHeight());

    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks(const Player* player, const Window* window, const World* world) {
    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        glBindVertexArray(chunk->_mesh->_vaoId);

        // Bind the VBO to the context
        glBindBuffer(GL_ARRAY_BUFFER, chunk->_mesh->_vboId);

        // Upload the vertex data to the GPU
        glBufferData(GL_ARRAY_BUFFER, VertexData::size() * chunk->_mesh->_vertices.size(), chunk->_mesh->_vertices.data(), GL_STATIC_DRAW);

        // Set the layout/stride information on the bound VAO for this data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexData::size(), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);

        glVertexAttribIPointer(1, 1, GL_INT, VertexData::size(), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribIPointer(2, 1, GL_INT, VertexData::size(), reinterpret_cast<void *>((3 * sizeof(float)) + sizeof(int)));
        glEnableVertexAttribArray(2);

        _blockShader.use();

        _blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());
        _blockShader.setMat4("uViewMatrix", player->getViewMatrix());
        _blockShader.setMat4("uProjectionMatrix", player->getProjectionMatrix(window));

        glDrawArrays(GL_TRIANGLES, 0, (chunk->_mesh->_vertices.size() * 3));
    }

    glBindVertexArray(0);
}

void RenderManager::render(const Player* player, const Window* window, const World* world) {
    renderBlocks(player, window, world);

    // Foreach of stored types, render.


}

