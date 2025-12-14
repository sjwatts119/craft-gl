#include "core/world.h"

World::World() {
    addTestChunks();
}

void World::addTestChunks() {
    // 0,0
    // 0,1
    // 1,0
    // 1,1

    for (int x = 0; x < 2; x++) {
        for (int z = 0; z < 2; z++) {
            const Coordinate coordinate {x, 0, z};

            Chunk chunk{coordinate};

            _chunks.insert({Coordinate {x, 0, z}, chunk});

            std::cout << "added test chunk to world at localised position {x: " << x << " z: " << z << "}" << std::endl;
        }
    }
}
