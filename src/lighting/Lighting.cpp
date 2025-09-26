#include "Lighting.hpp"
#include "src/Engine.hpp"
#include <memory>
#include <queue>

std::queue<Chunk*> Lighting::preBuildQueue;

Lighting::Lighting(AreaMap2D<Chunk>& chunks) 
  : chunks(chunks) {
    // solverR = std::make_unique<LightSolver>(chunks, 0);
    // solverG = std::make_unique<LightSolver>(chunks, 1);
    // solverB = std::make_unique<LightSolver>(chunks, 2);
    // solverS = std::make_unique<LightSolver>(chunks, 3);
}

Lighting::~Lighting() = default;

void Lighting::clear(){
    Chunk** chunks = Engine::pChunkMap->get_volume();
    int size = Engine::pChunkMap->size;

    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            Chunk* chunk = Engine::pChunkMap->firstBuffer->get(x, z);
            if (!chunk) continue;
            chunk->lightmap.clear();
        }
    }

    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            Chunk* chunk = Engine::pChunkMap->firstBuffer->get(x, z);
            if (!chunk) continue;
            Engine::pLighting->prebuildSkyLight(chunk);
        }
    }

    while (!Lighting::preBuildQueue.empty()) {
        Chunk* chunk = Lighting::preBuildQueue.front();
        Lighting::preBuildQueue.pop();
        Engine::pLighting->prebuildSkyLight(chunk);
    }
}


void Lighting::prebuildSkyLight(Chunk* chunk){

    chunk->lightmap.clear();
    int cx = chunk->X;
    int cz = chunk->Z;
    
    for (int z = 0; z < CHUNK_W; z++){
        for (int x = 0; x < CHUNK_W; x++){
            for (int y = CHUNK_H-1; y >= 0; y--){ 
                voxel vox = chunk->get_voxel(x, y, z);
                
                if (vox.id != 0) { 
                    break;
                }
                
                chunk->lightmap.setS(x, y, z, 15);
            }
        }
    }
    chunk->modified = true;
}


// void Lighting::buildSkyLight(int cx, int cz) {

//     Chunk* chunk = Engine::pChunkMap->get(cx, cy, cz);
//     if (chunk == nullptr) return;

//     for (int z = 0; z < CHUNK_W; z++){
//         for (int x = 0; x < CHUNK_W; x++){
//             int gx = x + cx * CHUNK_W;
//             int gz = z + cz * CHUNK_W;
//             for (int y = CHUNK_H-1; y >= 0; y--){
//                 int gy = y + cy * CHUNK_H;
//                 // while (gy > 0 && Engine::pVoxelStorage->get_voxel(gx, gy, gz).id != 0) { // !blockDefs[chunk->voxels[vox_index(x, y, z)].id]->lightPassing
//                 //     gy--;
//                 // }
                
//                 if (chunk->lightmap.getS(x, y, z) > 1) {
//                 uint8_t light = chunk->lightmap.getS(x, y, z);
//                     solverS->add(gx,gy+1,gz, light);
//                     for (; y >= 0; y--){
//                         solverS->add(gx+1,gy,gz, light);
//                         solverS->add(gx-1,gy,gz, light);
//                         solverS->add(gx,gy,gz+1, light);
//                         solverS->add(gx,gy,gz-1, light);
//                     }
//                 }
//             }
//         }
//     }
//     solverS->solve();
// }




