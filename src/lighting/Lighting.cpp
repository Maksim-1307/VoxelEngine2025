#include "Lighting.hpp"
#include "src/Engine.hpp"
#include <memory>
#include <algorithm>
#include <queue>

std::queue<Chunk*> Lighting::preBuildQueue;
std::mutex Lighting::lightingMtx;

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
    
    if (!chunk) return;

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

    std::lock_guard<std::mutex> lock(lightingMtx);

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        return;
    }

    bool update_neighbors = !chunk->update_caused_by_neighbor;

    std::array<Chunk*, 8> neighboring_chunks;
    neighboring_chunks[0] = Engine::pChunkMap->get(cx+1, cz);
    neighboring_chunks[1] = Engine::pChunkMap->get(cx-1, cz);
    neighboring_chunks[2] = Engine::pChunkMap->get(cx, cz+1);
    neighboring_chunks[3] = Engine::pChunkMap->get(cx, cz-1);
    neighboring_chunks[4] = Engine::pChunkMap->get(cx+1, cz+1);
    neighboring_chunks[5] = Engine::pChunkMap->get(cx+1, cz-1);
    neighboring_chunks[6] = Engine::pChunkMap->get(cx-1, cz+1);
    neighboring_chunks[7] = Engine::pChunkMap->get(cx-1, cz-1);

    // Compare edge hash before / after building light,
    // to update neighbors' light only if needed
    std::array<uint64_t, 4> edge_hash_before = chunk->lightmap.getCommitedHashes();

    // Building light of the current chunk
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

    // Seed cross-border propagation with adjacent chunks' border light
    Chunk* neighbor;
    // -X
    neighbor = Engine::pChunkMap->get(cx-1, cz);
    if (neighbor && neighbor->state >= LIGHTS_PRE_BUILT) {
        int gx = (cx-1)*CHUNK_W + CHUNK_W-1;
        for (int z = 0; z < CHUNK_W; z++) {
            int gz = cz*CHUNK_W + z;
            for (int y = 0; y < CHUNK_H; y++) {
                light l = neighbor->lightmap.get(CHUNK_W-1, y, z);
                if (l) solver->add(gx, y, gz, l);
            }
        }
        solver->solve();
    }
    
    // +X
    neighbor = Engine::pChunkMap->get(cx+1, cz);
    if (neighbor && neighbor->state >= LIGHTS_PRE_BUILT) {
        int gx = (cx+1)*CHUNK_W;
        for (int z = 0; z < CHUNK_W; z++) {
            int gz = cz*CHUNK_W + z;
            for (int y = 0; y < CHUNK_H; y++) {
                light l = neighbor->lightmap.get(0, y, z);
                if (l) solver->add(gx, y, gz, l);
            }
        }
        solver->solve();
    }
    
    // -Z
    neighbor = Engine::pChunkMap->get(cx, cz-1);
    if (neighbor && neighbor->state >= LIGHTS_PRE_BUILT) {
        int gz = (cz-1)*CHUNK_W + CHUNK_W-1;
        for (int x = 0; x < CHUNK_W; x++) {
            int gx = cx*CHUNK_W + x;
            for (int y = 0; y < CHUNK_H; y++) {
                light l = neighbor->lightmap.get(x, y, CHUNK_W-1);
                if (l) solver->add(gx, y, gz, l);
            }
        }
        solver->solve();
    }
    
    // +Z
    neighbor = Engine::pChunkMap->get(cx, cz+1);
    if (neighbor && neighbor->state >= LIGHTS_PRE_BUILT) {
        int gz = (cz+1)*CHUNK_W;
        for (int x = 0; x < CHUNK_W; x++) {
            int gx = cx*CHUNK_W + x;
            for (int y = 0; y < CHUNK_H; y++) {
                light l = neighbor->lightmap.get(x, y, 0);
                if (l) solver->add(gx, y, gz, l);
            }
        }
        solver->solve();
    }

    solver->solve();
    chunk->state = LIGHTS_BUILT;
    chunk->update_caused_by_neighbor = false;

    // Edge hash after
    std::array<uint64_t, 4> edge_hash_after;
    edge_hash_after[0] = chunk->lightmap.getHashOfPositiveXEdge(); // X+
    edge_hash_after[1] = chunk->lightmap.getHashOfNegativeXEdge(); // X-
    edge_hash_after[2] = chunk->lightmap.getHashOfPositiveZEdge(); // Z+
    edge_hash_after[3] = chunk->lightmap.getHashOfNegativeZEdge(); // Z-

    // Update if needed
    std::array<bool, 8> need_update;
    need_update[0] = edge_hash_before[0] != edge_hash_after[0]; // X+
    need_update[1] = edge_hash_before[1] != edge_hash_after[1]; // X-
    need_update[2] = edge_hash_before[2] != edge_hash_after[2]; // Z+
    need_update[3] = edge_hash_before[3] != edge_hash_after[3]; // Z-

    need_update[4] = need_update[0] || need_update[2]; // X+ Z+
    need_update[5] = need_update[0] || need_update[3]; // X+ Z-
    need_update[6] = need_update[1] || need_update[2]; // X- Z+
    need_update[7] = need_update[1] || need_update[3]; // X- Z-

    if (update_neighbors) {
        for (int i = 0; i < 8; i++) {
            if (need_update[i] || true) {
                Chunk* neighbor = neighboring_chunks[i];
                // std::lock_guard<std::mutex> neighborLock(neighbor->mtx); // makes very slow, deadlock probably
                if (neighbor->state > LIGHTS_BUILT) {
                    neighbor->state = LIGHTS_BUILT;
                }
                neighbor->update_caused_by_neighbor = true;
            }
        }
    }
}

void Lighting::onBlockSet(int x, int y, int z, uint8_t blockId) {

    std::lock_guard<std::mutex> lock(lightingMtx);

    solver->remove(x, y, z);
    solver->solve();

    if (blockId != 0) {
        Block& block = Block::getBlockByVoxelId(blockId);
        if (block.emissive) {
            light l = {0};
            l.setR(block.emission[0]);
            l.setG(block.emission[1]);
            l.setB(block.emission[2]);
            solver->add(x, y, z, l);
            solver->solve();
        }
    }
}

void Lighting::onChunkLoaded(int cx, int cz, bool expand) {

    Profiler p ("onChunkLoaded");

    std::lock_guard<std::mutex> lock(lightingMtx);

    Chunk* chunk = Engine::pChunkMap->get(cx, cz);
    if (chunk == nullptr) {
        return;
    }
    // std::lock_guard<std::mutex> lock(chunk->mtx);

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
                    if (l.emitting()) {
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
                    if (l.emitting()) {
                        solver->add(gx, y, gz, l);
                    }
                }
            }
        }
    }
    solver->solve();
}

void Lighting::solve() {
    // std::cout << "Solving lighting...\n";
    // std::lock_guard<std::mutex> lock(lightingMtx);
    // solver->solve();
}