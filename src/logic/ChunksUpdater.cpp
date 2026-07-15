#include "ChunksUpdater.hpp"
#include "src/Engine.hpp"
#include "src/logic/ThreadPool.hpp"

void ChunksUpdater::update_immediately(ChunkPos chunkPos) {

    ThreadPool::get_instance().enqueue([chunkPos]() {
        
        std::cout << "Updating chunk at " << chunkPos.first() << ", " << chunkPos.second() << "\n";

        static std::mutex sharedMtx;

        int x = chunkPos.first();
        int z = chunkPos.second();
        Chunk* chunk = Engine::pChunkMap->get(x, z);
        if (!chunk) return;

        std::lock_guard chunkLock(chunk->mtx);

        if (chunk->state < MODIFIED) {
            std::cout << "WARNING: called chunk update on chunk that is not fully generated\n";
            return;
        }
        
        try {

            // building or updating sky light if its needed
            if (Settings::HARD_LOADING || !chunk->renderer || chunk->state < VISIBLE) {

                // Fix: if we do prebuildSkyLight here, it causes edge case issues
                // If we dont, sky light is not updated on block set
                if (chunk->state == MODIFIED && !Settings::RECURSIVE_LIGHTING) {
                    std::lock_guard sharedLock(sharedMtx);
                    Engine::pLighting->prebuildSkyLight(chunk);
                }

                if (Settings::RECURSIVE_LIGHTING) {
                    std::lock_guard sharedLock(sharedMtx);
                    Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
                    Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
                }

                sptr<Mesh> mesh;
                {
                    std::lock_guard sharedLock(sharedMtx);
                    mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
                }
                if (!mesh) return;
                
                chunk->pendingMesh = std::move(mesh);

            }
            chunk->state = MESH_BUILT;
            chunk->isDirty = false;
        } catch (...) {
            std::cerr << "ERROR: Failed to update chunk at " 
                    << x << ", " << z << "\n";
        }
    });
}