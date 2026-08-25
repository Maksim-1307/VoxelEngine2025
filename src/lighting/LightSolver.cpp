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

    light l = chunk->lightmap.get(ix, y, iz);
    uint16_t emVal = emission.value;
    if (!l.emitting() && emVal == 0) return;

    uint16_t curVal = chunk->lightmap.map.get(ix, y, iz).value;


    uint16_t newVal = curVal;
    bool canPush = false;

    // R
    uint8_t em0 = (emVal >> 12) & 0xF;
    uint8_t cv0 = (curVal >> 12) & 0xF;
    if (em0 >= cv0) {
        if (em0 > cv0) { newVal = (newVal & 0x0FFF) | (em0 << 12); }
        canPush = true;
    }
    // G
    uint8_t em1 = (emVal >> 8) & 0xF;
    uint8_t cv1 = (curVal >> 8) & 0xF;
    if (em1 >= cv1) {
        if (em1 > cv1) { newVal = (newVal & 0xF0FF) | (em1 << 8); }
        canPush = true;
    }
    // B
    uint8_t em2 = (emVal >> 4) & 0xF;
    uint8_t cv2 = (curVal >> 4) & 0xF;
    if (em2 >= cv2) {
        if (em2 > cv2) { newVal = (newVal & 0xFF0F) | (em2 << 4); }
        canPush = true;
    }
    // S
    uint8_t em3 = emVal & 0xF;
    uint8_t cv3 = curVal & 0xF;
    if (em3 >= cv3) {
        if (em3 > cv3) { newVal = (newVal & 0xFFF0) | em3; }
        canPush = true;
    }

    if (!canPush) return;

    chunk->lightmap.map.set(ix, y, iz, {newVal});
    addqueue.push({x, y, z, {newVal}});
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

        uint16_t ev = entry.value.value;

        for (int i = 0; i < 6; i++) {
            int imul3 = i*3;
            int x = entry.x+coords[imul3];
            int y = entry.y+coords[imul3+1];
            int z = entry.z+coords[imul3+2];

            if (y < 0 || y >= CHUNK_H) continue;

            int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
            int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
            Chunk* chunk = chunks.get(cx, cz);

            if (!chunk) continue;

            std::lock_guard<std::mutex> chunkLock(chunk->mtx);

            int lx = x & 15;
            int lz = z & 15;

            uint16_t cur = chunk->lightmap.map.get(lx, y, lz).value;
            if (cur == 0) continue;

            bool cascade = false;
            bool readd = false;

            // R
            uint8_t ev0 = (ev >> 12) & 0xF;
            uint8_t cv0 = (cur >> 12) & 0xF;
            if (ev0 > 0 && cv0 != 0 && cv0 == ev0 - 1) cascade = true;
            else if (ev0 > 0 && cv0 >= ev0) readd = true;

            // G
            uint8_t ev1 = (ev >> 8) & 0xF;
            uint8_t cv1 = (cur >> 8) & 0xF;
            if (ev1 > 0 && cv1 != 0 && cv1 == ev1 - 1) cascade = true;
            else if (ev1 > 0 && cv1 >= ev1) readd = true;

            // B
            uint8_t ev2 = (ev >> 4) & 0xF;
            uint8_t cv2 = (cur >> 4) & 0xF;
            if (ev2 > 0 && cv2 != 0 && cv2 == ev2 - 1) cascade = true;
            else if (ev2 > 0 && cv2 >= ev2) readd = true;

            // S
            uint8_t ev3 = ev & 0xF;
            uint8_t cv3 = cur & 0xF;
            if (ev3 > 0 && cv3 != 0 && cv3 == ev3 - 1) cascade = true;
            else if (ev3 > 0 && cv3 >= ev3) readd = true;

            if (cascade) {
                uint16_t newVal = cur;
                if (ev0 > 0 && cv0 != 0 && cv0 == ev0 - 1) newVal &= 0x0FFF;
                if (ev1 > 0 && cv1 != 0 && cv1 == ev1 - 1) newVal &= 0xF0FF;
                if (ev2 > 0 && cv2 != 0 && cv2 == ev2 - 1) newVal &= 0xFF0F;
                if (ev3 > 0 && cv3 != 0 && cv3 == ev3 - 1) newVal &= 0xFFF0;

                chunk->lightmap.map.set(lx, y, lz, {newVal});

                uint16_t remVal = 0;
                if (ev0 > 0 && cv0 != 0 && cv0 == ev0 - 1) remVal |= (cv0 << 12);
                if (ev1 > 0 && cv1 != 0 && cv1 == ev1 - 1) remVal |= (cv1 << 8);
                if (ev2 > 0 && cv2 != 0 && cv2 == ev2 - 1) remVal |= (cv2 << 4);
                if (ev3 > 0 && cv3 != 0 && cv3 == ev3 - 1) remVal |= cv3;

                remqueue.push({x, y, z, {remVal}});
            }
            else if (readd) {
                addqueue.push({x, y, z, {cur}});
            }
        }
    }

    while (!addqueue.empty()){
        const lightentry entry = addqueue.front();
        addqueue.pop();

        uint16_t ev = entry.value.value;

        for (int i = 0; i < 6; i++) {
            int imul3 = i*3;
            int x = entry.x+coords[imul3];
            int y = entry.y+coords[imul3+1];
            int z = entry.z+coords[imul3+2];

            if (y < 0 || y >= CHUNK_H) continue;

            int cx = (x >= 0) ? (x >> 4) : (x - 15) / 16;
            int cz = (z >= 0) ? (z >> 4) : (z - 15) / 16;
            Chunk* chunk = chunks.get(cx, cz);

            if (!chunk) continue;

            int lx = x & 15;
            int lz = z & 15;

            uint16_t cur = chunk->lightmap.map.get(lx, y, lz).value;
            if (cur == ev) continue;

            voxel v = chunk->get_voxel(lx, y, lz);
            if (!Block::getBlockByVoxelId(v.id).lightPassing) continue;

            uint16_t newVal = cur;
            bool changed = false;

            // R
            uint8_t ev0 = (ev >> 12) & 0xF;
            if (ev0 > 1) {
                uint8_t cv0 = (cur >> 12) & 0xF;
                if (cv0 + 2 <= ev0) {
                    newVal = (newVal & 0x0FFF) | ((ev0 - 1) << 12);
                    changed = true;
                }
            }
            // G
            uint8_t ev1 = (ev >> 8) & 0xF;
            if (ev1 > 1) {
                uint8_t cv1 = (cur >> 8) & 0xF;
                if (cv1 + 2 <= ev1) {
                    newVal = (newVal & 0xF0FF) | ((ev1 - 1) << 8);
                    changed = true;
                }
            }
            // B
            uint8_t ev2 = (ev >> 4) & 0xF;
            if (ev2 > 1) {
                uint8_t cv2 = (cur >> 4) & 0xF;
                if (cv2 + 2 <= ev2) {
                    newVal = (newVal & 0xFF0F) | ((ev2 - 1) << 4);
                    changed = true;
                }
            }
            // S
            uint8_t ev3 = ev & 0xF;
            if (ev3 > 1) {
                uint8_t cv3 = cur & 0xF;
                if (cv3 + 2 <= ev3) {
                    newVal = (newVal & 0xFFF0) | (ev3 - 1);
                    changed = true;
                }
            }

            if (changed) {
                chunk->lightmap.map.set(lx, y, lz, {newVal});
                addqueue.push({x, y, z, {newVal}});
            }
        }
    }
}
