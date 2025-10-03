#include "Lighting.hpp"
#include "src/Engine.hpp"
#include <memory>
#include <queue>

std::queue<Chunk*> Lighting::preBuildQueue;

Lighting::Lighting(AreaMap2D<Chunk>& chunks) 
  : chunks(chunks) {
    solverR = std::make_unique<LightSolver>(chunks, 0);
    solverG = std::make_unique<LightSolver>(chunks, 1);
    solverB = std::make_unique<LightSolver>(chunks, 2);
    solverS = std::make_unique<LightSolver>(chunks, 3);
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
            Engine::pLighting->prebuildSkyLight(chunk);
            // Engine::pLighting->buildSkyLight(x, z);
        }
    }
    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            // Chunk* chunk = Engine::pChunkMap->firstBuffer->get(x, z);
            // if (!chunk) continue;
            // chunk->lightmap.clear();
            // Engine::pLighting->buildSkyLight(x, z);
        }
    }

    // while (!Lighting::preBuildQueue.empty()) {
    //     Chunk* chunk = Lighting::preBuildQueue.front();
    //     Lighting::preBuildQueue.pop();
    //     Engine::pLighting->prebuildSkyLight(chunk);
    // }
}


void Lighting::prebuildSkyLight(Chunk* chunk){

    // if (chunk->state < STRUCTURES_GENERATED) {
    //     // std::cout << "WARNING: prebuildSkyLight called for a non fully generated chunk! The chunk state is " 
    //     //     << (int)chunk->state << "\n";
    //     return;
    // }
    // if (chunk->state > STRUCTURES_GENERATED) {
    //     // std::cout << "WARNING: prebuildSkyLight called for a chunk already lighted! The chunk state is " 
    //     //     << (int)chunk->state << "\n";
    //     return;
    // }
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
    chunk->state = LIGHTS_PRE_BUILT;
}


void Lighting::buildSkyLight(int cx, int cz) {

    auto& solverR = *this->solverR;
    auto& solverG = *this->solverG;
    auto& solverB = *this->solverB;
    auto& solverS = *this->solverS;

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        // logger.error() << "attempted to build lights to chunk missing in local matrix";
        return;
    }
    // for (uint y = 0; y < CHUNK_H; y++){
    //     for (uint z = 0; z < CHUNK_W; z++){
    //         for (uint x = 0; x < CHUNK_W; x++){
    //             const voxel& vox = chunk->voxels[(y * CHUNK_W + z) * CHUNK_W + x];
    //             const Block* block = blockDefs[vox.id];
    //             int gx = x + cx * CHUNK_W;
    //             int gz = z + cz * CHUNK_W;
    //             if (block->rt.emissive){
    //                 solverR.add(gx,y,gz,block->emission[0]);
    //                 solverG.add(gx,y,gz,block->emission[1]);
    //                 solverB.add(gx,y,gz,block->emission[2]);
    //             }
    //         }
    //     }
    // }


    for (int x = 0; x < CHUNK_W; x ++) {
        for (int y = 0; y < CHUNK_H; y++) {
            for (int z = 0; z < CHUNK_W; z++) {
                int gx = x + cx * CHUNK_W;
                int gz = z + cz * CHUNK_W;
                light light = chunk->lightmap.get(x, y, z);
                if (light){
                    solverR.add(gx,y,gz, light.getR());
                    solverG.add(gx,y,gz, light.getG());
                    solverB.add(gx,y,gz, light.getB());
                    solverS.add(gx,y,gz, light.getS());
                }
            }
        }
    }
    solverR.solve();
    solverG.solve();
    solverB.solve();
    solverS.solve();
    chunk->state = LIGHTS_BUILT;
}

void Lighting::onChunkLoaded(int cx, int cz, bool expand) {
    auto& solverR = *this->solverR;
    auto& solverG = *this->solverG;
    auto& solverB = *this->solverB;
    auto& solverS = *this->solverS;

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        // logger.error() << "attempted to build lights to chunk missing in local matrix";
        return;
    }
    // for (uint y = 0; y < CHUNK_H; y++){
    //     for (uint z = 0; z < CHUNK_W; z++){
    //         for (uint x = 0; x < CHUNK_W; x++){
    //             const voxel& vox = chunk->voxels[(y * CHUNK_W + z) * CHUNK_W + x];
    //             const Block* block = blockDefs[vox.id];
    //             int gx = x + cx * CHUNK_W;
    //             int gz = z + cz * CHUNK_W;
    //             if (block->rt.emissive){
    //                 solverR.add(gx,y,gz,block->emission[0]);
    //                 solverG.add(gx,y,gz,block->emission[1]);
    //                 solverB.add(gx,y,gz,block->emission[2]);
    //             }
    //         }
    //     }
    // }

    if (expand) {
        for (int x = 0; x < CHUNK_W; x += CHUNK_W-1) {
            for (int y = 0; y < CHUNK_H; y++) {
                for (int z = 0; z < CHUNK_W; z++) {
                    int gx = x + cx * CHUNK_W;
                    int gz = z + cz * CHUNK_W;
                    light light = chunk->lightmap.get(x, y, z);
                    if (light){
                        solverR.add(gx,y,gz, light.getR());
                        solverG.add(gx,y,gz, light.getG());
                        solverB.add(gx,y,gz, light.getB());
                        solverS.add(gx,y,gz, light.getS());
                    }
                }
            }
        }
        for (int z = 0; z < CHUNK_W; z += CHUNK_W-1) {
            for (int y = 0; y < CHUNK_H; y++) {
                for (int x = 0; x < CHUNK_W; x++) {
                    int gx = x + cx * CHUNK_W;
                    int gz = z + cz * CHUNK_W;
                    light light = chunk->lightmap.get(x, y, z);
                    if (light){
                        solverR.add(gx,y,gz, light.getR());
                        solverG.add(gx,y,gz, light.getG());
                        solverB.add(gx,y,gz, light.getB());
                        solverS.add(gx,y,gz, light.getS());
                    }
                }
            }
        }
    }
    solverR.solve();
    solverG.solve();
    solverB.solve();
    solverS.solve();
}


