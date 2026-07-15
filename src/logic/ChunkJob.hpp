// #pragma once
// #include "Job.hpp"

// class ChunkJob : public Job {
// public:
//     ChunkJob(int x, int z) : x(x), z(z) {}

//     void execute() override {

//         Chunk* chunk = Engine::pChunkMap->get(x, z);
//         if (!chunk) return;
//         if (chunk->state < MODIFIED) {
//             std::cout << "WARNING: called chunk update on chunk that is not fully generated\n";
//             return;
//         }
        
//         try {

//             // building or updating sky light if its needed
//             if (Settings::HARD_LOADING || !chunk->renderer || chunk->state < VISIBLE) {

//                 // Fix: if we do prebuildSkyLight here, it causes edge case issues
//                 // If we dont, sky light is not updated on block set
//                 if (chunk->state == MODIFIED && !Settings::RECURSIVE_LIGHTING)
//                     Engine::pLighting->prebuildSkyLight(chunk);

//                 if (Settings::RECURSIVE_LIGHTING) {
//                     Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
//                     Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
//                 }

//                 sptr<Mesh> mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
//                 if (!mesh) return;
                
//                 if (chunk->renderer) chunk->renderer = nullptr;
//                 chunk->renderer = make_uptr<MeshRenderer>(std::move(mesh), MeshType::MESH3D);
//                 chunk->renderer->transform = glm::translate(
//                     glm::mat4(1.0f),
//                     glm::vec3(chunk->X * CHUNK_W, 0, chunk->Z * CHUNK_W)
//                 );
                
//             }
//             chunk->state = VISIBLE;
//             chunk->isDirty = false;
//         } catch (...) {
//             std::cerr << "ERROR: Failed to update chunk at " 
//                     << x << ", " << z << "\n";
//         }
//     }
// private:
//     Chunk* chunk;
// };