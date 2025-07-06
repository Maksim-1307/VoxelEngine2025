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

            int chunkX = std::floor(x / CHUNK_W);
            int chunkY = std::floor(y / CHUNK_H);
            int chunkZ = std::floor(z / CHUNK_W);

            std::cout << chunkX << " " << chunkY << " " << chunkZ << "\n";

            int blockX = MOD(x, 16);
            int blockY = MOD(y, 16);
            int blockZ = MOD(z, 16);

            return chunksMap->get(chunkX, chunkY, chunkZ)->get_voxel(blockX, blockY, blockZ);
        };

        // void set_voxel(int x, int y, int z, voxel vox){

        //     int chunkX = std::floor(x / CHUNK_W);
        //     int chunkY = std::floor(y / CHUNK_H);
        //     int chunkZ = std::floor(z / CHUNK_W);

        //     int blockX = MOD(x, 16);
        //     int blockY = MOD(y, 16);
        //     int blockZ = MOD(z, 16);

        //     chunksMap->get(chunkX, chunkY, chunkZ)->set_voxel(blockX, blockY, blockZ, vox);
        // };

    private:
    AreaMap3D<Chunk>* chunksMap;

};
   