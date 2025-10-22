#pragma once

#include <iostream>
#include <cmath>
#include "src/logic/AreaMap3D.hpp"
#include "src/voxels/Chunk.hpp"

#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

/* wrapper for AreaMap3D operates on blocks, not chunks */

class VoxelStorage{
    public:

        VoxelStorage(AreaMap3D<Chunk> *chunksMap)
            : chunksMap(chunksMap) 
        {};

        voxel get_voxel(int x, int y, int z){

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            Chunk* ch = chunksMap->get(chunkX, chunkY, chunkZ);
            if (ch == nullptr) {
                // std::cout << "ERROR: Chunk out of bounds at " << chunkX << " " << chunkY << " " << chunkZ << "\n";
                return {2, 0};
            }

            return ch->get_voxel(blockX, blockY, blockZ);
        };

        void set_voxel(int x, int y, int z, voxel vox){

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            chunksMap->get(chunkX, chunkY, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
        };
        bool set_voxel_soft(int x, int y, int z, voxel vox){

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            if (!chunksMap->is_inside(chunkX, chunkY, chunkZ))
                return false;

            chunksMap->get(chunkX, chunkY, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
            chunksMap->get(chunkX, chunkY, chunkZ)->state = MODIFIED;

            if (blockX == 0) chunksMap->get(chunkX-1, chunkY, chunkZ)->state = MODIFIED;
            if (blockX == CHUNK_W-1) chunksMap->get(chunkX+1, chunkY, chunkZ)->state = MODIFIED;
            if (blockZ == 0) chunksMap->get(chunkX, chunkY, chunkZ-1)->state = MODIFIED;
            if (blockZ == CHUNK_W-1) chunksMap->get(chunkX, chunkY, chunkZ+1)->state = MODIFIED;
            if (blockY == 0) chunksMap->get(chunkX, chunkY-1, chunkZ)->state = MODIFIED;
            if (blockY == CHUNK_H-1) chunksMap->get(chunkX, chunkY+1, chunkZ)->state = MODIFIED;
            return true;
        };

        light get_light(int x, int y, int z) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkY, chunkZ)->lightmap.get(blockX, blockY, blockZ);

        };

        uint8_t get_light(int x, int y, int z, int channel) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkY, chunkZ)->lightmap.get(blockX, blockY, blockZ, channel);

        };

    private:
    int get_chunk_coord(int coord, int chunk_size) const {
        return std::floor((float)coord / chunk_size);
    }

    int get_block_coord(int coord, int chunk_size) const {
        int result = coord % chunk_size;
        if (result < 0) {
            result += chunk_size;
        }
        return result;
    }
    AreaMap3D<Chunk>* chunksMap;

};
   