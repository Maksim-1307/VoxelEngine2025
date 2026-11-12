#include "ChunksUpdater.hpp"
#include "src/Engine.hpp"
#include "src/graphics/ChunkMeshBuilder.hpp"
#include "src/logic/ThreadPool.hpp"

void ChunksUpdater::queue_chunk(ChunkPos chunkPos) {
    Chunk* chunk = Engine::pChunkMap->get(chunkPos.first(), chunkPos.second());
    if (!chunk) return;
    chunk->isQueued = true;
    chunksToUpdate.insert(chunkPos);
}

void ChunksUpdater::update_light(ChunkPos chunkPos) {
    
        int x = chunkPos.first();
        int z = chunkPos.second();
        Chunk* chunk = Engine::pChunkMap->get(x, z);
        if (!chunk) return;
        chunk->isQueued = true;

        // Lock briefly to check state
        std::unique_lock chunkLock(chunk->mtx);
        if (chunk->state < MODIFIED) {
            // Chunk is not prepared yet (terrain/structures are built
            // synchronously in load_around). Release it so it can be
            // queued again once prepared instead of getting stuck.
            chunk->isQueued = false;
            return;
        }
        chunkLock.unlock();

        try {
            std::lock_guard lock(chunk->mtx);
            Engine::pLighting->prebuildSkyLight(chunk);
            
            if (Settings::RECURSIVE_LIGHTING) {
                Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
                Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
            }
            // Actually, not yet. The lighting calculation is in the queue and will
            // only be completed once solve_all_light is called and finished.
            // Cannot build the mesh until that has happened.
            chunk->state = LIGHTS_BUILT;
        } catch (...) {
            std::cerr << "ERROR: Failed to update lighting for chunk at " 
                    << chunkPos.first() << ", " << chunkPos.second() << "\n";
        }
}

void ChunksUpdater::update_mesh(ChunkPos chunkPos) {
        int x = chunkPos.first();
        int z = chunkPos.second();
        Chunk* chunk = Engine::pChunkMap->get(x, z);
        if (!chunk) return;
        chunk->isQueued = true;
        std::unique_lock chunkLock(chunk->mtx);
        if (chunk->state < MODIFIED) {
            // Chunk is not prepared yet (terrain/structures are built
            // synchronously in load_around). Release it so it can be
            // queued again once prepared instead of getting stuck.
            chunk->isQueued = false;
            return;
        }
        chunkLock.unlock();
        try {
            std::lock_guard lock(chunk->mtx);
            sptr<Mesh> mesh;
            {
                ChunkMeshBuilder builder;
                mesh = builder.buildMesh(*chunk);
            }
            if (!mesh) return;
            chunk->pendingMesh = std::move(mesh);
            chunk->isDirty = false;
            chunk->isQueued = false;
            chunk->state = MESH_BUILT;
        } catch (...) {
            std::cerr << "ERROR: Failed to update mesh for chunk at " 
                    << chunkPos.first() << ", " << chunkPos.second() << "\n";
        }
}

void ChunksUpdater::update_immediately(ChunkPos chunkPos) {

    // ThreadPool::get_instance().enqueue([chunkPos]() {

    //     static std::mutex sharedMtx;

    //     int x = chunkPos.first();
    //     int z = chunkPos.second();
    //     Chunk* chunk = Engine::pChunkMap->get(x, z);
    //     if (!chunk) return;
    //     chunk->isQueued = true;

    //     // Lock briefly to check state
    //     std::unique_lock chunkLock(chunk->mtx);
    //     if (chunk->state < MODIFIED) {
    //         // Chunk is not prepared yet (terrain/structures are built
    //         // synchronously in load_around). Release it so it can be
    //         // queued again once prepared instead of getting stuck.
    //         chunk->isQueued = false;
    //         return;
    //     }
    //     chunkLock.unlock();

    //     try {

    //         {
    //             // std::lock_guard sharedLock(sharedMtx);
    //             Engine::pLighting->prebuildSkyLight(chunk);
                
    //             if (Settings::RECURSIVE_LIGHTING) {
    //                 Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
    //                 Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
    //             }
    //         }

    //         sptr<Mesh> mesh;
    //         {
    //             // std::lock_guard sharedLock(sharedMtx);
    //             ChunkMeshBuilder builder;
    //             mesh = builder.buildMesh(*chunk);
    //         }
    //         if (!mesh) {
    //             chunk->isQueued = false;
    //             return;
    //         }

    //         chunkLock.lock();
    //         chunk->pendingMesh = std::move(mesh);
    //         chunk->state = MESH_BUILT;
    //         chunk->isDirty = false;
    //         chunk->isQueued = false;
    //         chunkLock.unlock();

    //     } catch (...) {
    //         std::cerr << "ERROR: Failed to update chunk at " 
    //                 << x << ", " << z << "\n";
    //         if (chunk) {
    //             chunk->isQueued = false;
    //         }
    //     }
    // });
}

void ChunksUpdater::solve_all_light() {
    
    // ThreadPool::get_instance().enqueue([]() {

    //     static std::mutex sharedMtx;

    //     {
    //         std::lock_guard sharedLock(sharedMtx);
    //         Engine::pLighting->solve();
    //     }
    // });
}