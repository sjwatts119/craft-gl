#include <render/renderManager.h>

#include "core/debug.h"
#include "render/buffer/aabbData.h"

RenderManager::RenderManager(const Window* window) {
    // Viewport
    glViewport(0, 0, window->getWidth(), window->getHeight());

    // Clear color (sky blue)
    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    // Back-face culling
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks(const Player* player, const Window* window, const World* world) const {
    _shaderManager._blockShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._blockShader.setInt("uBlockTextures", 0);

    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        chunk->_mesh->bind();

        _shaderManager._blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());
        _shaderManager._blockShader.setMat4("uViewMatrix", player->getViewMatrix());
        _shaderManager._blockShader.setMat4("uProjectionMatrix", player->getProjectionMatrix(window));

        _shaderManager._blockShader.setLight("uSun" , world->getSun());

        chunk->_mesh->render();
    }

    glBindVertexArray(0);
}

void RenderManager::renderInterface(const Window *window, const Interface *interface) const {
    interface->_crosshair.bind();

    _shaderManager._crosshairShader.use();
    _shaderManager._crosshairShader.setMat4("uModelMatrix", Crosshair::localToWorldMatrix(window));

    Crosshair::render();

    glBindVertexArray(0);
}

void RenderManager::renderDebug(const Player *player, const Window* window, const Debug *debug) const {
    debug->bind();

    _shaderManager._debugShader.use();

    _shaderManager._debugShader.setMat4("uViewMatrix", player->getViewMatrix());
    _shaderManager._debugShader.setMat4("uProjectionMatrix", player->getProjectionMatrix(window));

    debug->render();

    glBindVertexArray(0);
}

void RenderManager::render(const Player* player, const Window* window, const World* world, const Interface* interface, const Debug* debug) const {
    renderBlocks(player, window, world);

    renderInterface(window, interface);

    renderDebug(player, window, debug);
}
