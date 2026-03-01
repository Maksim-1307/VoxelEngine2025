#pragma once

#include <iostream>
#include <vector>
#include "src/logic/pointers.hpp"

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include "Mesh.hpp"
#include "src/voxels/Block.hpp"
#include "src/voxels/Chunk.hpp"

#define ATLAS_SIZE 6
#define GET_VOXEL_INDEX(x, y, z) ((x) * CHUNK_H * CHUNK_W + (y) * CHUNK_W + (z))


class ChunkMeshBuilder {
public:
    ChunkMeshBuilder (){}

    sptr<Mesh> buildMesh(Chunk& chunk);

    uint16_t packRGBS(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
        return (a & 0xF) << 12 | (b & 0xF) << 8 | (c & 0xF) << 4 | (d & 0xF);
    }

    float pack_to_float(uint16_t value){
        uint32_t tmp = value;  
        float result;
        std::memcpy(&result, &tmp, sizeof(tmp)); 
        return result; 
    }
    
    float pack_to_float(uint16_t a, uint16_t b){
        uint32_t combined = (static_cast<uint32_t>(b) << 16) | a;
        float result;
        memcpy(&result, &combined, sizeof(float));
        return result;
    }

private: 

    void CubeModel(int x, int y, int z);

    std::array<bool, 6> opened_around(int x, int y, int z);
    int adjacent(int face);
    
    void vertex(float x, float y, float z, float u, float v);
    void index(uint a, uint b, uint c, uint d, uint e, uint f);
    uint16_t calculate_light();
    inline voxel get_voxel_fast(int x, int y, int z); // uses chached chunks
    inline light get_light_fast(int x, int y, int z);
    inline bool is_in_bounds(int x, int y, int z);

    std::vector<float> vertices = {};
    std::vector<uint> indices = {};

    int indexOffset = 0;
    int _x, _y, _z;
    int _face;
    int chunkX, chunkY, chunkZ;
    Chunk* chunk;

    // Caching voxels of chunks 
    const voxel* currD;
    const voxel* nxD;
    const voxel* pxD;
    const voxel* nzD;
    const voxel* pzD;
};
