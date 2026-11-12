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

    // call each tick
    void update() {
        batchCounter++;
        if (batchCounter >= BATCH_INTERVAL) {
            solve_all_light();
            std::cout << "Updating " << chunksToUpdate.size() << " chunks\n";
            for (ChunkPos chunkPos : chunksToUpdate) {
                update_light(chunkPos);
                update_mesh(chunkPos);
                // update_immediately(chunkPos);
            }
            chunksToUpdate.clear();
            batchCounter = 0;
        }
        
    }

    void queue_chunk(ChunkPos chunkPos);

    void update_light(ChunkPos chunkPos);

    void update_mesh(ChunkPos chunkPos);

    // update modified chunk
    void update_immediately(ChunkPos chunkPos);

    // build recursive lighting for all chunks in the queue
    void solve_all_light();

private:
    ChunksUpdater() {}
    ~ChunksUpdater() {}

    int batchCounter = 0;
    std::unordered_set<ChunkPos> chunksToUpdate;
};