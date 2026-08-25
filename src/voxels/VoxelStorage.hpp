#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/logic/BlockUpdater.hpp"

#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

/* wrapper for AreaMap3D operates on blocks, not chunks */

class VoxelStorage{
    public:

        VoxelStorage(AreaMap2D<Chunk> *chunksMap)
            : chunksMap(chunksMap) 
        {};

        voxel get_voxel(int x, int y, int z) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = 0; //get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            Chunk* ch = chunksMap->get(chunkX, chunkZ);
            if (ch == nullptr) {
                // std::cout << "ERROR: Chunk out of bounds at " << chunkX << " " << chunkY << " " << chunkZ << "\n";
                return {0, 0};
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

            chunksMap->get(chunkX, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
        };

        bool set_voxel_soft(
            int x, int y, int z, 
            voxel vox, 
            bool triggerCallbacks = true, 
            bool immediately = false // highest priority in the workers queue
        );

        light get_light(int x, int y, int z) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkZ)->lightmap.get(blockX, blockY, blockZ);

        };

        uint8_t get_light(int x, int y, int z, int channel) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkZ)->lightmap.get(blockX, blockY, blockZ, channel);

        };

        // Remake !!! Only for testing 
        void set_light(int x, int y, int z, uint8_t value) const {

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            chunksMap->get(chunkX, chunkZ)->lightmap.setS(blockX, blockY, blockZ, value);

        };

        bool find_in_radius(int x, int y, int z, int radius, std::function<bool(voxel)> condition) const {
            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    for (int dz = -radius; dz <= radius; dz++) {
                        if (dx*dx + dy*dy + dz*dz <= radius*radius) {
                            voxel v = get_voxel(x + dx, y + dy, z + dz);
                            if (condition(v)) {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
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
    AreaMap2D<Chunk>* chunksMap;

};
   