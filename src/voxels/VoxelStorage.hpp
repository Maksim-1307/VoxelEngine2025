#pragma once

#include <iostream>
#include <cmath>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"

#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

// wrapper for AreaMap3D operates on blocks, not chunks
class VoxelStorage{
    public:

        VoxelStorage(AreaMap2D<Chunk> *chunksMap)
            : chunksMap(chunksMap) 
        {};

        voxel get_voxel(int x, int y, int z){

            int chunkX = get_chunk_coord(x, CHUNK_W);
            int chunkY = 0; //get_chunk_coord(y, CHUNK_H);
            int chunkZ = get_chunk_coord(z, CHUNK_W);

            int blockX = get_block_coord(x, CHUNK_W);
            int blockY = get_block_coord(y, CHUNK_H);
            int blockZ = get_block_coord(z, CHUNK_W);

            Chunk* ch = chunksMap->get(chunkX, chunkZ);
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

            chunksMap->get(chunkX, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
        };

        // light get_light(int x, int y, int z) const {

        //     int chunkX = get_chunk_coord(x, CHUNK_W);
        //     int chunkY = get_chunk_coord(y, CHUNK_H);
        //     int chunkZ = get_chunk_coord(z, CHUNK_W);

        //     int blockX = get_block_coord(x, CHUNK_W);
        //     int blockY = get_block_coord(y, CHUNK_H);
        //     int blockZ = get_block_coord(z, CHUNK_W);

        //     return chunksMap->get(chunkX, chunkZ)->lightmap.get(blockX, blockY, blockZ);

        // };

        // uint8_t get_light(int x, int y, int z, int channel) const {

        //     int chunkX = get_chunk_coord(x, CHUNK_W);
        //     int chunkY = get_chunk_coord(y, CHUNK_H);
        //     int chunkZ = get_chunk_coord(z, CHUNK_W);

        //     int blockX = get_block_coord(x, CHUNK_W);
        //     int blockY = get_block_coord(y, CHUNK_H);
        //     int blockZ = get_block_coord(z, CHUNK_W);

        //     return chunksMap->get(chunkX, chunkZ)->lightmap.get(blockX, blockY, blockZ, channel);

        // };

        // // Remake !!! Only for testing 
        // void set_light(int x, int y, int z, uint8_t value) const {

        //     int chunkX = get_chunk_coord(x, CHUNK_W);
        //     int chunkY = get_chunk_coord(y, CHUNK_H);
        //     int chunkZ = get_chunk_coord(z, CHUNK_W);

        //     int blockX = get_block_coord(x, CHUNK_W);
        //     int blockY = get_block_coord(y, CHUNK_H);
        //     int blockZ = get_block_coord(z, CHUNK_W);

        //     chunksMap->get(chunkX, chunkZ)->lightmap.setS(blockX, blockY, blockZ, value);

        // };

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
   