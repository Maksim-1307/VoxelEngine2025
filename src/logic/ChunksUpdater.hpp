#pragma once

#include <iostream>
#include <unordered_set>

#include "ChunkPos.hpp"

// update low priority chunks every BATCH_INTERVAL ticks
constexpr int BATCH_INTERVAL = 3;

class ChunksUpdater {
public:
    static ChunksUpdater& get_instance() {
        static ChunksUpdater instance;
        return instance;
    }

    void update() {}

    void update_pre_light(ChunkPos chunkPos);
    void update_light(ChunkPos chunkPos);
    void update_mesh(ChunkPos chunkPos);
    void generate_terrain(ChunkPos chunkPos);
    void generate_ambient(ChunkPos chunkPos);

private:
    ChunksUpdater() {}
    ~ChunksUpdater() {}

    int batchCounter = 0;
    std::unordered_set<ChunkPos> chunksToUpdate;
};