#include "ChunksUpdater.hpp"
#include "src/Engine.hpp"
#include "src/logic/ThreadPool.hpp"

void ChunksUpdater::update_immediately(ChunkPos chunkPos) {

    ThreadPool::get_instance().enqueue([chunkPos]() {

        static std::mutex sharedMtx;

        int x = chunkPos.first();
        int z = chunkPos.second();
        Chunk* chunk = Engine::pChunkMap->get(x, z);
        if (!chunk) return;

        // Lock briefly to check state
        std::unique_lock chunkLock(chunk->mtx);
        if (chunk->state < MODIFIED) {
            return;
        }
        chunkLock.unlock();

        try {

            {
                std::lock_guard sharedLock(sharedMtx);
                Engine::pLighting->prebuildSkyLight(chunk);
                
                if (Settings::RECURSIVE_LIGHTING) {
                    Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
                    Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
                }
            }

            sptr<Mesh> mesh;
            {
                std::lock_guard sharedLock(sharedMtx);
                mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
            }
            if (!mesh) return;

            chunkLock.lock();
            chunk->pendingMesh = std::move(mesh);
            chunk->state = MESH_BUILT;
            chunk->isDirty = false;
            chunkLock.unlock();

        } catch (...) {
            std::cerr << "ERROR: Failed to update chunk at " 
                    << x << ", " << z << "\n";
        }
    });
}