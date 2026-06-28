#include "LightSolver.hpp"

#include "src/Engine.hpp"

LightSolver::LightSolver(AreaMap2D<Chunk>& chunks)
    : chunks(chunks)
{
}

void LightSolver::add(int x, int y, int z){
    light l = Engine::pVoxelStorage->get_light(x, y, z);
    add(x, y, z, l);
}

void LightSolver::add(int x, int y, int z, light emission){
    if (emission.value == 0)
        return;

    if (y < 0 || y >= CHUNK_H) return;

    int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
    int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
    int ix = x & 15;
    int iz = z & 15;

    Chunk* chunk = chunks.get(cx, cz);
    if (chunk == nullptr)
        return;

    light current = chunk->lightmap.map.get(ix, y, iz);

    light merged = current;
    bool canPush = false;
    for (int c = 0; c < 4; c++) {
        uint8_t em = emission.get(c);
        if (em >= current.get(c)) {
            if (em > current.get(c)) {
                merged.set(c, em);
            }
            canPush = true;
        }
    }
    if (!canPush) return;

    chunk->lightmap.map.set(ix, y, iz, merged);
    addqueue.push({x, y, z, merged});
}


void LightSolver::remove(int x, int y, int z) {
    if (y < 0 || y >= CHUNK_H) return;

    int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
    int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
    int ix = x & 15;
    int iz = z & 15;

    Chunk* chunk = chunks.get(cx, cz);
    if (chunk == nullptr)
        return;

    light current = chunk->lightmap.map.get(ix, y, iz);
    if (current.value == 0)
        return;

    remqueue.push({x, y, z, current});
    chunk->lightmap.map.set(ix, y, iz, {0});
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

            if (y < 0 || y >= CHUNK_H) continue;

            int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
            int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
            Chunk* chunk = chunks.get(cx, cz);

            if (chunk) {
                int lx = x & 15;
                int lz = z & 15;

                light current = chunk->lightmap.map.get(lx, y, lz);
                if (current.value == 0) continue;

                uint8_t cascadeMask = 0;
                uint8_t readdMask = 0;

                for (int c = 0; c < 4; c++) {
                    uint8_t cv = current.get(c);
                    uint8_t ev = entry.value.get(c);
                    if (ev > 0 && cv != 0 && cv == ev - 1) {
                        cascadeMask |= (1 << c);
                    } else if (ev > 0 && cv >= ev) {
                        readdMask |= (1 << c);
                    }
                }

                if (cascadeMask) {
                    light newlight = current;
                    for (int c = 0; c < 4; c++) {
                        if (cascadeMask & (1 << c)) {
                            newlight.set(c, 0);
                        }
                    }
                    chunk->lightmap.map.set(lx, y, lz, newlight);

                    light remval = {0};
                    for (int c = 0; c < 4; c++) {
                        if (cascadeMask & (1 << c)) {
                            remval.set(c, current.get(c));
                        }
                    }
                    remqueue.push({x, y, z, remval});
                }
                else if (readdMask) {
                    addqueue.push({x, y, z, current});
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

            if (y < 0 || y >= CHUNK_H) continue;

            int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
            int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
            Chunk* chunk = chunks.get(cx, cz);

            if (chunk) {
                int lx = x & 15;
                int lz = z & 15;

                light current = chunk->lightmap.map.get(lx, y, lz);
                voxel v = chunk->get_voxel(lx, y, lz);
                bool lightPassing = Block::getBlockByVoxelId(v.id).lightPassing;

                if (!lightPassing) continue;

                light newlight = current;
                bool changed = false;

                for (int c = 0; c < 4; c++) {
                    uint8_t cv = current.get(c);
                    uint8_t ev = entry.value.get(c);
                    if (ev > 1 && cv + 2 <= ev) {
                        newlight.set(c, ev - 1);
                        changed = true;
                    }
                }

                if (changed) {
                    chunk->lightmap.map.set(lx, y, lz, newlight);
                    addqueue.push({x, y, z, newlight});
                }
            }
        }
    }
}
