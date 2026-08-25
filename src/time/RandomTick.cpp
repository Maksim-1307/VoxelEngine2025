#include "RandomTick.hpp"
#include "src/Engine.hpp"
#include "src/logic/BlockUpdater.hpp"
#include "src/logic/Settings.hpp"

pcg32 RandomTick::rng;

void RandomTick::tick() {
    // for each chunk choose a random voxel and update it
    if (!Settings::BLOCKS_BEHAVIOR) return;
    for (Chunk* chunk : Engine::pChunksController->get_loaded_chunks()) {
        for (int i = 0; i < UPDATES_PER_TICK; i++) {
            int x = chunk->X * CHUNK_W + rng(CHUNK_W);
            int y = rng(CHUNK_H);
            int z = chunk->Z * CHUNK_W + rng(CHUNK_W);
            BlockUpdater::get_instance().on_random_tick(x, y, z);
        }
    }
}