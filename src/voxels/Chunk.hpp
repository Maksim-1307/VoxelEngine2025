#pragma once

#include <iostream>

#include "src/logic/Array3D.hpp"
#include "voxel.hpp"

#define CHUNK_W 16
#define CHUNK_H 256


class Chunk{
public:
    Chunk(){};
    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;
    void set_voxel(size_t x, size_t y, size_t z, const voxel voxel)
    {
        voxels.set(x, y, z, voxel);
    }
    const voxel get_voxel(size_t x, size_t y, size_t z) const
    {
        return voxels.get(x, y, z);
    }

private:
    Array3D<voxel> voxels = Array3D<voxel>(CHUNK_W, CHUNK_H, CHUNK_W);
};
