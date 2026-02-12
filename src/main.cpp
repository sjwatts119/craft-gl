#include <render/window.h>

#include "render/renderable/debug.h"
#include "core/player.h"
#include "core/world.h"
#include "render/renderManager.h"
#include "core/craft.h"

int main() {
    Craft::setup();

    while (Craft::window->open()) {
        Craft::window->update();

        Craft::renderManager->clear();

        for (int i = 0; i < Craft::window->getTicksElapsed(); i++) {
            Craft::player->tick();
            Craft::world->tick();
        }

        Craft::world->update();
        Craft::player->update();
        Craft::debug->update();

        Craft::renderManager->render();

        Craft::window->poll();
        Craft::window->swapBuffers();
    }

    Craft::window->destroy();
}
