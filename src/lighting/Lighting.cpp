#include "Lighting.hpp"
#include "src/Engine.hpp"
#include <memory>
#include <queue>

std::queue<Chunk*> Lighting::preBuildQueue;

Lighting::Lighting(AreaMap2D<Chunk>& chunks) 
  : chunks(chunks) {
    solver = std::make_unique<LightSolver>(chunks);
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
        }
    }

}


void Lighting::prebuildSkyLight(Chunk* chunk){

    Profiler t("prebuildSkyLight");

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

    Profiler p("buildSkyLight");

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        return;
    }

    for (int x = 0; x < CHUNK_W; x ++) {
        for (int y = 0; y < CHUNK_H; y++) {
            for (int z = 0; z < CHUNK_W; z++) {
                int gx = x + cx * CHUNK_W;
                int gz = z + cz * CHUNK_W;
                light l = chunk->lightmap.get(x, y, z);
                if (l){
                    solver->add(gx, y, gz, l);
                }
            }
        }
    }
    solver->solve();
    chunk->state = LIGHTS_BUILT;
}

void Lighting::onChunkLoaded(int cx, int cz, bool expand) {

    Profiler p ("onChunkLoaded");

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        return;
    }
    for (uint y = 0; y < CHUNK_H; y++){
        for (uint z = 0; z < CHUNK_W; z++){
            for (uint x = 0; x < CHUNK_W; x++){
                const voxel& vox = chunk->get_voxel(x, y, z);
                const Block& block = Block::getBlockByVoxelId(vox.id);
                int gx = x + cx * CHUNK_W;
                int gz = z + cz * CHUNK_W;
                if (block.emissive){
                    light l = {0};
                    l.setR(block.emission[0]);
                    l.setG(block.emission[1]);
                    l.setB(block.emission[2]);
                    solver->add(gx, y, gz, l);
                }
            }
        }
    }

    if (expand) {
        for (int x = 0; x < CHUNK_W; x += CHUNK_W-1) {
            for (int y = 0; y < CHUNK_H; y++) {
                for (int z = 0; z < CHUNK_W; z++) {
                    int gx = x + cx * CHUNK_W;
                    int gz = z + cz * CHUNK_W;
                    light l = chunk->lightmap.get(x, y, z);
                    if (l){
                        solver->add(gx, y, gz, l);
                    }
                }
            }
        }
        for (int z = 0; z < CHUNK_W; z += CHUNK_W-1) {
            for (int y = 0; y < CHUNK_H; y++) {
                for (int x = 0; x < CHUNK_W; x++) {
                    int gx = x + cx * CHUNK_W;
                    int gz = z + cz * CHUNK_W;
                    light l = chunk->lightmap.get(x, y, z);
                    if (l){
                        solver->add(gx, y, gz, l);
                    }
                }
            }
        }
    }
    solver->solve();
}
