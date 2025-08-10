#include "LightSolver.hpp"

#include "src/Engine.hpp"

LightSolver::LightSolver(AreaMap3D<Chunk>& chunks, int channel)
    : chunks(chunks)
{}

void LightSolver::add(int x, int y, int z){
    unsigned char light = Engine::pVoxelStorage->get_light(x, y, z, channel);
    add(x, y, z, light);
}
void LightSolver::add(int x, int y, int z, unsigned char emission){
    if (emission <= 1)
        return;
    int cx = std::floor((float)x / CHUNK_W);
    int cy = std::floor((float)y / CHUNK_H);
    int cz = std::floor((float)z / CHUNK_W);
    int ix = x - cx * CHUNK_W;
    int iy = y - cy * CHUNK_H;
    int iz = z - cz * CHUNK_W;
    Chunk* chunk = chunks.get(cx, cy, cz);
    if (chunk == nullptr)
        return;
    unsigned char light = chunk->lightmap.get(ix, iy, iz, channel);
    if (emission < light) return;

    addqueue.push(lightentry {x, y, z, emission});

    chunk->modified = true;
    chunk->lightmap.set(ix, iy, iz, channel, emission);
}



void LightSolver::remove(int x, int y, int z) {
    int cx = std::floor((float)x / CHUNK_W);
    int cy = std::floor((float)y / CHUNK_H);
    int cz = std::floor((float)z / CHUNK_W);
    int ix = x - cx * CHUNK_W;
    int iy = y - cy * CHUNK_H;
    int iz = z - cz * CHUNK_W;
    Chunk* chunk = chunks.get(cx, cy, cz);
    if (chunk == nullptr)
        return;

    uint8_t light = chunk->lightmap.get(ix, iy, iz, channel);
    if (light == 0){
        return;
    }
    remqueue.push(lightentry {x, y, z, light});
    chunk->lightmap.set(ix, iy, iz, channel, 0);
}

void LightSolver::solve(){
    const int coords[] = {
            0, 0, 1,
            0, 0,-1,
            0, 1, 0,
            0,-1, 0,
            1, 0, 0,
           -1, 0, 0
    };

    while (!remqueue.empty()){
        const lightentry entry = remqueue.front();
        remqueue.pop();

        for (int i = 0; i < 6; i++) {
            int imul3 = i*3;
            int x = entry.x+coords[imul3];
            int y = entry.y+coords[imul3+1];
            int z = entry.z+coords[imul3+2];
            
            int cx = std::floor((float)x / CHUNK_W);
            int cy = std::floor((float)y / CHUNK_H);
            int cz = std::floor((float)z / CHUNK_W);
            Chunk* chunk = chunks.get(cx, cy, cz);

            if (chunk) {
                int lx = x - cx * CHUNK_W;
                int ly = y - cy * CHUNK_H;
                int lz = z - cz * CHUNK_W;
                chunk->modified = true;

                uint8_t light = chunk->lightmap.get(lx, ly, lz, channel);
                if (light != 0 && light == entry.light-1){
                    voxel vox = Engine::pVoxelStorage->get_voxel(x, y, z);
                    if (vox.id != 0) {
                        // const Block* block = blockDefs[vox->id];
                        // if (uint8_t emission = block->emission[channel]) {
                        //     addqueue.push(lightentry {x, y, z, emission});
                        //     chunk->lightmap.set(lx, ly, lz, channel, emission);
                        // }
                        // else chunk->lightmap.set(lx, ly, lz, channel, 0);
                        chunk->lightmap.set(lx, ly, lz, channel, 0);
                    }
                    else chunk->lightmap.set(lx, ly, lz, channel, 0);
                    remqueue.push(lightentry {x, y, z, light});
                }
                else if (light >= entry.light){
                    addqueue.push(lightentry {x, y, z, light});
                }
            }
        }
    }

    while (!addqueue.empty()){
        const lightentry entry = addqueue.front();
        addqueue.pop();

        for (int i = 0; i < 6; i++) {
            int imul3 = i*3;
            int x = entry.x+coords[imul3];
            int y = entry.y+coords[imul3+1];
            int z = entry.z+coords[imul3+2];

            int cx = std::floor((float)x / CHUNK_W);
            int cy = std::floor((float)y / CHUNK_H);
            int cz = std::floor((float)z / CHUNK_W);
            Chunk* chunk = chunks.get(cx, cy, cz);

            if (chunk) {
                int lx = x - cx * CHUNK_W;
                int ly = y - cy * CHUNK_H;
                int lz = z - cz * CHUNK_W;
                chunk->modified = true;

                uint8_t light = chunk->lightmap.get(lx, ly, lz, channel);
                voxel v = chunk->get_voxel(lx, ly, lz);
                // const Block* block = blockDefs[v.id];
                // if (block->lightPassing && light+2 <= entry.light){
                //     chunk->lightmap.set(
                //         x-cx*CHUNK_W, y-cy*CHUNK_H, z-cz*CHUNK_W, 
                //         channel, 
                //         entry.light-1);
                //     addqueue.push(lightentry {x, y, z, uint8_t(entry.light-1)});
                // }
                if (v.id == 0 && light+2 <= entry.light){
                    chunk->lightmap.set(
                        x-cx*CHUNK_W, y-cy*CHUNK_H, z-cz*CHUNK_W, 
                        channel, 
                        entry.light-1);
                    addqueue.push(lightentry {x, y, z, uint8_t(entry.light-1)});
                }
            }
        }
    }
}
