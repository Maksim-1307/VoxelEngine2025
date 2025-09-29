#pragma once

#include <iostream>

#include "src/logic/Array3D.hpp"
#include "voxel.hpp"
#include "src/graphics/MeshRenderer.hpp"
#include "src/lighting/Lightmap.hpp"

#define CHUNK_W 16
#define CHUNK_H 64

/*

PADDING 2
1) Terrain generation. 

PADDING 1
2) Structures generation. 
3) Lights pre-building. 
4) Mesh building.  

VISIBLE
5) Lights building. 
Can be shown

*/

enum ChunkState { 
    INITIALIZED = 0,
    // padding 0
    TERRAIN_GENERATED = 1, 
    // padding 1
    STRUCTURES_GENERATED = 2, 
    MODIFIED = 3,
    LIGHTS_PRE_BUILT = 4,
    LIGHTS_BUILT = 5,
    MESH_BUILT = 6,
    // shown
    VISIBLE = 7
};

class Chunk{
public:
    Chunk(){
        Chunk::chunks += 1;
        lightmap.clear();
        state = ChunkState::INITIALIZED;
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
    ChunkState state;

private:
    Array3D<voxel> voxels = Array3D<voxel>(CHUNK_W, CHUNK_H, CHUNK_W);
};
