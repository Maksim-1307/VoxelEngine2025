#pragma once

#include <iostream>
#include <vector>

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"
#include "src/voxels/Block.hpp"
#include "src/voxels/Chunk.hpp"

#define ATLAS_SIZE 8


class ChunkMeshBuilder {
public:
    ChunkMeshBuilder (){}

    Mesh* buildMesh(Chunk& chunk) {

        vertices.clear();
        indices.clear();
        vertices.reserve(1000);
        indices.reserve(500);

        this->chunk = &chunk;

        indexOffset = 0;

        int chunkX = 0;
        int chunkZ = 0;
        int blockX = chunkX * CHUNK_W;
        int blockZ = chunkZ * CHUNK_W;

        for (_x = 0; _x < CHUNK_W; _x++)
        {
            for (_z = 0; _z < CHUNK_W; _z++)
            {
                for (_y = 0; _y < CHUNK_H; _y++)
                {
                    if (chunk.get_voxel(_x, _y, _z).id != 0){
                        std::cout << "solid vox\n";
                        CubeModel(_x, _y, _z);
                    }
                }
            }
        }

        return new Mesh(vertices.data(), vertices.size(), indices.data(), indices.size());
    }

private: 

    void CubeModel(int x, int y, int z){
        if (x == 0 || y == 0 || z == 0 || x == 16 || z == 16 || y == 256) return;
        std::array<bool, 6> openedFaces = opened_around(x, y, z);
        // Block block = Block.GetBlockByVoxelId(VoxelStorage.GetVoxel(x, y, z).Id);

        for (int face = 0; face < 6; face++)
        {
            if (openedFaces[face])
            {
                // (uint x, uint y) blockFaceUV = block.GetUV((byte)face); // standart
                // (float x, float y) uv = ((float)blockFaceUV.x / ATLAS_SIZE, (float)blockFaceUV.y / ATLAS_SIZE); // normalized
                glm::vec2 uv = glm::vec2(0.0f, 0.0f);
                _face = face;
                switch (face)
                {
                    // X+
                    case 0:
                        vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        vertex(1.0f, 0.0f, 1.0f, uv.x, uv.y);
                        vertex(1.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        break;
                    // X-
                    case 1:
                        vertex(0.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y);
                        vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(0.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        break;
                    // Y+
                    case 2:
                        vertex(0.0f, 1.0f, 0.0f, uv.x, uv.y);
                        vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 1.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        break;
                    // Y-
                    case 3:
                        vertex(0.0f, 0.0f, 0.0f, uv.x, uv.y);
                        vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 0.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        break;
                    // Z+
                    case 4:
                        vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y);
                        vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 1.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 0.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        break;
                    // Z-
                    case 5:
                        vertex(0.0f, 0.0f, 0.0f, uv.x, uv.y);
                        vertex(0.0f, 1.0f, 0.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
                        vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
                        break;
                }

                // direct and reverse order (when polygon must be rendered from other side)
                if (face % 2 == 0)
                {
                    index(0, 1, 3, 1, 2, 3);
                }
                else
                {
                    index(3, 1, 0, 3, 2, 1);
                }
            }
        }
    }

    // std::array<bool, 6> opened_around(int x, int y, int z)
    // {
    //     std::array<bool, 6> opened;// = new bool[6];
    //     opened[0] = Block::getBlockByVoxelId(chunk->get_voxel(x + 1, y, z).id).opened_faces[adjacent(0)];
    //     opened[1] = Block::getBlockByVoxelId(chunk->get_voxel(x - 1, y, z).id).opened_faces[adjacent(1)];
    //     opened[2] = Block::getBlockByVoxelId(chunk->get_voxel(x, y + 1, z).id).opened_faces[adjacent(2)];
    //     opened[3] = Block::getBlockByVoxelId(chunk->get_voxel(x, y - 1, z).id).opened_faces[adjacent(3)];
    //     opened[4] = Block::getBlockByVoxelId(chunk->get_voxel(x, y, z + 1).id).opened_faces[adjacent(4)];
    //     opened[5] = Block::getBlockByVoxelId(chunk->get_voxel(x, y, z - 1).id).opened_faces[adjacent(5)];

    //     return opened;
    // }

    std::array<bool, 6> opened_around(int x, int y, int z)
    {
        std::array<bool, 6> opened;
        opened[0] = chunk->get_voxel(x + 1, y, z).id == 0;
        opened[1] = chunk->get_voxel(x - 1, y, z).id == 0;
        opened[2] = chunk->get_voxel(x, y + 1, z).id == 0;
        opened[3] = chunk->get_voxel(x, y - 1, z).id == 0;
        opened[4] = chunk->get_voxel(x, y, z + 1).id == 0;
        opened[5] = chunk->get_voxel(x, y, z - 1).id == 0;

        return opened;
    }

    int adjacent(int face) 
    {
        if (face % 2 == 0) return face + 1;
        return face - 1;
    }

    void vertex(float x, float y, float z, float u, float v)
    {
        const float newVertex[] = {_x + x, _y + y, _z + z, u, v};
        vertices.insert(vertices.end(), std::begin(newVertex), std::end(newVertex));
    }

    void index(uint a, uint b, uint c, uint d, uint e, uint f)
    {
        indices.push_back(indexOffset + a);
        indices.push_back(indexOffset + b);
        indices.push_back(indexOffset + c);
        indices.push_back(indexOffset + d);
        indices.push_back(indexOffset + e);
        indices.push_back(indexOffset + f);
        indexOffset += 4;
    }

    std::vector<float> vertices = {};
    std::vector<uint> indices = {};
    unsigned int indexOffset = 0;
    unsigned int _x, _y, _z;
    unsigned int _face;
    Chunk* chunk;
};
