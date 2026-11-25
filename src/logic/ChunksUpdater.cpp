#include "ChunksUpdater.hpp"
#include "src/Engine.hpp"
#include "src/graphics/ChunkMeshBuilder.hpp"
#include "src/logic/ThreadPool.hpp"

std::mutex ChunksUpdater::trees_mutex;

void ChunksUpdater::update_pre_light(ChunkPos chunkPos) {
    int x = chunkPos.first();
    int z = chunkPos.second();
    sptr<Chunk> chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return;
    chunk->isQueued = true;

    // // Lock briefly to check state
    // std::unique_lock chunkLock(chunk->mtx);
    // if (chunk->state < MODIFIED) {
    //     chunk->isQueued = false;
    //     return;
    // }
    // chunkLock.unlock();

    try {
        std::lock_guard lock(chunk->mtx);
        Engine::pLighting->prebuildSkyLight(chunk);
        
        chunk->state = LIGHTS_PRE_BUILT;
    } catch (...) {
        std::cerr << "ERROR: Failed to update pre-lighting for chunk at " 
                << chunkPos.first() << ", " << chunkPos.second() << "\n";
    }
}

void ChunksUpdater::update_light(ChunkPos chunkPos) {
    int x = chunkPos.first();
    int z = chunkPos.second();
    sptr<Chunk> chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return;
    chunk->isQueued = true;

    // // Lock briefly to check state
    // std::unique_lock chunkLock(chunk->mtx);
    // if (chunk->state < MODIFIED) {
    //     chunk->isQueued = false;
    //     return;
    // }
    // chunkLock.unlock();

    try {
        std::lock_guard lock(chunk->mtx);
        // Engine::pLighting->prebuildSkyLight(chunk);
        
        if (Settings::RECURSIVE_LIGHTING) {
            Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
            Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
        }
        chunk->state = LIGHTS_BUILT;
    } catch (...) {
        std::cerr << "ERROR: Failed to update lighting for chunk at " 
                << chunkPos.first() << ", " << chunkPos.second() << "\n";
    }
}

void ChunksUpdater::update_mesh(ChunkPos chunkPos) {
    int x = chunkPos.first();
    int z = chunkPos.second();
    sptr<Chunk> chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return;
    chunk->isQueued = true;
    std::unique_lock chunkLock(chunk->mtx);
    if (chunk->state < MODIFIED) {
        chunk->isQueued = false;
        return;
    }
    chunkLock.unlock();
    try {
        std::lock_guard lock(chunk->mtx);
        sptr<Mesh> mesh;
        {
            ChunkMeshBuilder builder;
            mesh = builder.buildMesh(chunk);
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

void ChunksUpdater::generate_terrain(ChunkPos chunkPos) {
    Engine::pGenerator->generate_at(chunkPos.x, chunkPos.z);
}

void ChunksUpdater::generate_ambient(ChunkPos chunkPos) {
    std::lock_guard lock(trees_mutex);
    Engine::pGenerator->generate_ambient(chunkPos.x, chunkPos.z);
}