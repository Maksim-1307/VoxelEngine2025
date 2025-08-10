#pragma once

#include <iostream>

#include "src/logic/Array3D.hpp"
#include "voxel.hpp"
#include "src/graphics/MeshRenderer.hpp"
#include "src/lighting/Lightmap.hpp"

#define CHUNK_W 16
#define CHUNK_H 16


class Chunk{
public:
    Chunk(){
        Chunk::chunks += 1;
        lightmap.clear();
    };
    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;
    ~Chunk(){
        Chunk::chunks -= 1;
    };

    MeshRenderer* renderer;
    int X, Y, Z;
    Lightmap lightmap = Lightmap(CHUNK_W, CHUNK_H, CHUNK_W);

    void set_voxel(size_t x, size_t y, size_t z, const voxel voxel)
    {
        voxels.set(x, y, z, voxel);
    }
    const voxel get_voxel(size_t x, size_t y, size_t z) const
    {
        return voxels.get(x, y, z);
    }
    static int chunks;
    bool modified = false;

private:
    Array3D<voxel> voxels = Array3D<voxel>(CHUNK_W, CHUNK_H, CHUNK_W);
};
