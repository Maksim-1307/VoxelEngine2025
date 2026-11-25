#include "VoxelStorage.hpp"
#include "src/Engine.hpp"
#include "src/logic/BlockUpdater.hpp"

bool VoxelStorage::set_voxel_soft(
    int x, int y, int z, 
    voxel vox, 
    bool triggerCallbacks, 
    bool immediately
) {

    int chunkX = get_chunk_coord(x, CHUNK_W);
    int chunkY = get_chunk_coord(y, CHUNK_H);
    int chunkZ = get_chunk_coord(z, CHUNK_W);

    int blockX = get_block_coord(x, CHUNK_W);
    int blockY = get_block_coord(y, CHUNK_H);
    int blockZ = get_block_coord(z, CHUNK_W);

    if (blockY < 0 || blockY >= CHUNK_H || !chunksMap->is_inside(chunkX, chunkZ))
        return false;

    sptr<Chunk> chunk = chunksMap->get(chunkX, chunkZ);
    if (chunk == nullptr) return false;

    chunk->set_voxel(blockX, blockY, blockZ, vox);
    chunk->state = MODIFIED;
    chunk->update_caused_by_neighbor = false;

    if (triggerCallbacks) {
        BlockUpdater::get_instance().on_block_set(x, y, z);
        BlockUpdater::get_instance().on_block_set(x-1, y, z);
        BlockUpdater::get_instance().on_block_set(x+1, y, z);
        BlockUpdater::get_instance().on_block_set(x, y+1, z);
        BlockUpdater::get_instance().on_block_set(x, y-1, z);
        BlockUpdater::get_instance().on_block_set(x, y, z-1);
        BlockUpdater::get_instance().on_block_set(x, y, z+1);
    }

    auto mark_modified = [this](int cx, int cz) {
        sptr<Chunk> neighbor = chunksMap->get(cx, cz);
        if (neighbor != nullptr) neighbor->state = MODIFIED;
    };

    if (blockX == 0)         mark_modified(chunkX-1, chunkZ);
    if (blockX == CHUNK_W-1) mark_modified(chunkX+1, chunkZ);
    if (blockZ == 0)         mark_modified(chunkX, chunkZ-1);
    if (blockZ == CHUNK_W-1) mark_modified(chunkX, chunkZ+1);

    // for (int dx = -1; dx <= 1; dx++) {
    //     for (int dz = -1; dz <= 1; dz++) {
    //         if (dx == 0 && dz == 0) continue;
    //         if (chunksMap->is_inside(chunkX + dx, chunkZ + dz))
    //             chunksMap->get(chunkX + dx, chunkZ + dz)->state = MODIFIED;
    //     }
    // }
    return true;
};