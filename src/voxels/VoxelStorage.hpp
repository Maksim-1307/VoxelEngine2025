#pragma once

#include <iostream>
#include <cmath>
#include "src/logic/AreaMap3D.hpp"
#include "src/voxels/Chunk.hpp"

#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

// wrapper for AreaMap3D operates on blocks, not chunks
class VoxelStorage{
    public:

        VoxelStorage(AreaMap3D<Chunk> *chunksMap)
            : chunksMap(chunksMap) 
        {};

        voxel get_voxel(int x, int y, int z){

            int chunkX = std::floor((float)x / CHUNK_W);
            int chunkY = std::floor((float)y / CHUNK_H);
            int chunkZ = std::floor((float)z / CHUNK_W);

            int blockX = MOD(x, CHUNK_W);
            int blockY = MOD(y, CHUNK_H);
            int blockZ = MOD(z, CHUNK_W);

            Chunk* ch = chunksMap->get(chunkX, chunkY, chunkZ);
            if (ch == nullptr) {
                // std::cout << "ERROR: Chunk out of bounds at " << chunkX << " " << chunkY << " " << chunkZ << "\n";
                return {2, 0};
            }

            return ch->get_voxel(blockX, blockY, blockZ);
        };

        void set_voxel(int x, int y, int z, voxel vox){

            int chunkX = std::floor((float)x / CHUNK_W);
            int chunkY = std::floor((float)y / CHUNK_H);
            int chunkZ = std::floor((float)z / CHUNK_W);

            int blockX = MOD(x, 16);
            int blockY = MOD(y, 16);
            int blockZ = MOD(z, 16);

            chunksMap->get(chunkX, chunkY, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
        };

        light get_light(int x, int y, int z) const {

            int chunkX = std::floor((float)x / CHUNK_W);
            int chunkY = std::floor((float)y / CHUNK_H);
            int chunkZ = std::floor((float)z / CHUNK_W);

            int blockX = MOD(x, CHUNK_W);
            int blockY = MOD(y, CHUNK_H);
            int blockZ = MOD(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkY, chunkZ)->lightmap.get(blockX, blockY, blockZ);

        };

        uint8_t get_light(int x, int y, int z, int channel) const {

            int chunkX = std::floor((float)x / CHUNK_W);
            int chunkY = std::floor((float)y / CHUNK_H);
            int chunkZ = std::floor((float)z / CHUNK_W);

            int blockX = MOD(x, CHUNK_W);
            int blockY = MOD(y, CHUNK_H);
            int blockZ = MOD(z, CHUNK_W);

            return chunksMap->get(chunkX, chunkY, chunkZ)->lightmap.get(blockX, blockY, blockZ, channel);

        };

    private:
    AreaMap3D<Chunk>* chunksMap;

};
   