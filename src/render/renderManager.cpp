#include <render/renderManager.h>

#include "renderable/debug.h"

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

void RenderManager::renderBlocks(const Player* player, const Window* window, World* world) const {
    _shaderManager._blockShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._blockShader.setInt("uBlockTextures", 0);

    for (auto& [chunkCoordinate, chunk]: world->_chunks) {
        chunk->_mesh->bind();

        _shaderManager._blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());
        _shaderManager._blockShader.setMat4("uViewMatrix", player->getCamera()->getViewMatrix());
        _shaderManager._blockShader.setMat4("uProjectionMatrix", player->getCamera()->getProjectionMatrix(window));

        _shaderManager._blockShader.setLight("uSun" , world->getSun());

        chunk->_mesh->render();
    }

    glBindVertexArray(0);
}

void RenderManager::renderInterface(const Window *window, Interface *interface) const {
    interface->_crosshair.bind();

    _shaderManager._crosshairShader.use();
    _shaderManager._crosshairShader.setMat4("uModelMatrix", Crosshair::localToWorldMatrix(window));

    interface->_crosshair.render();

    glBindVertexArray(0);
}

void RenderManager::renderDebug(const Player *player, const Window* window, Debug *debug) const {
    debug->bind();

    _shaderManager._debugShader.use();

    _shaderManager._debugShader.setMat4("uViewMatrix", player->getCamera()->getViewMatrix());
    _shaderManager._debugShader.setMat4("uProjectionMatrix", player->getCamera()->getProjectionMatrix(window));

    debug->render();

    glBindVertexArray(0);
}

void RenderManager::render(const Player* player, const Window* window, World* world, Interface* interface, Debug* debug) const {
    renderBlocks(player, window, world);

    renderInterface(window, interface);

    // renderDebug(player, window, debug);
}
