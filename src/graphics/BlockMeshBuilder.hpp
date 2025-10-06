#pragma once
#include "src/logic/pointers.hpp"

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#define ATLAS_SIZE 6

class Block;
class Mesh;

/* Is used to build a mesh for a separrate block, like icons or items */
/* Rendeing functions should be same for each block model as in ChunkMeshBuilder */
class BlockMeshBuilder {
    public:
    BlockMeshBuilder();
    uptr<Mesh> buildMesh(Block& block);
    private:

    void CubeModel(Block& block);
    float pack_to_float(uint16_t a, uint16_t b){
        uint32_t combined = (static_cast<uint32_t>(b) << 16) | a;
        float result;
        memcpy(&result, &combined, sizeof(float));
        return result;
    }
    std::vector<float> vertices = {};
    std::vector<uint> indices = {};
    int indexOffset = 0;
    int _face;
    void vertex(float x, float y, float z, float u, float v);
    void index(uint a, uint b, uint c, uint d, uint e, uint f);
};