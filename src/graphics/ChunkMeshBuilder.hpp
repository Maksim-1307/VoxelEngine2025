#pragma once

#include <iostream>
#include <vector>

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include "Mesh.hpp"
#include "src/voxels/Block.hpp"
#include "src/voxels/Chunk.hpp"

#define ATLAS_SIZE 6


class ChunkMeshBuilder {
public:
    ChunkMeshBuilder (){}

    Mesh* buildMesh(Chunk& chunk);

    uint16_t packRGBS(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
        return (a & 0xF) << 12 | (b & 0xF) << 8 | (c & 0xF) << 4 | (d & 0xF);
    }
    float pack_to_float(uint16_t value){
        uint32_t tmp = value;  
        float result;
        std::memcpy(&result, &tmp, sizeof(tmp)); 
        return result; 
    }

private: 

    void CubeModel(int x, int y, int z);

    std::array<bool, 6> opened_around(int x, int y, int z);
    int adjacent(int face);
    
    void vertex(float x, float y, float z, float u, float v);
    void index(uint a, uint b, uint c, uint d, uint e, uint f);

    std::vector<float> vertices = {};
    std::vector<uint> indices = {};
    int indexOffset = 0;
    int _x, _y, _z;
    int _face;
    int chunkX, chunkY, chunkZ;
    Chunk* chunk;
};
