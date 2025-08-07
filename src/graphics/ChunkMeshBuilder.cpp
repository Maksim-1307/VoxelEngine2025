#include "ChunkMeshBuilder.hpp"
#include "src/Engine.hpp"

Mesh* ChunkMeshBuilder::buildMesh(Chunk &chunk)
{
    vertices.clear();
    indices.clear();
    vertices.reserve(1000);
    indices.reserve(500);

    this->chunk = &chunk;

    indexOffset = 0;

    this->chunkX = this->chunk->X * CHUNK_W;
    this->chunkY = this->chunk->Y * CHUNK_H;
    this->chunkZ = this->chunk->Z * CHUNK_W;

    for (_x = 0; _x < CHUNK_W; _x++)
    {
        for (_y = 0; _y < CHUNK_H; _y++)
        {
            for (_z = 0; _z < CHUNK_W; _z++)
            {
                if (Engine::pVoxelStorage->get_voxel(_x + chunkX, _y + chunkY, _z + chunkZ).id != 0)
                {
                    CubeModel(_x, _y, _z);
                }
            }
        }
    }
    return new Mesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void ChunkMeshBuilder::CubeModel(int x, int y, int z)
{

    std::array<bool, 6> openedFaces = opened_around(x, y, z);
    Block block = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY, z + chunkZ).id);

    for (int face = 0; face < 6; face++)
    {
        if (openedFaces[face])
        {
            size_t UVx = std::get<0>(block.getUV(face));
            size_t UVy = std::get<1>(block.getUV(face));
            glm::vec2 uv = glm::vec2((float)UVx / ATLAS_SIZE, (float)UVy / ATLAS_SIZE);
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

std::array<bool, 6> ChunkMeshBuilder::opened_around(int x, int y, int z)
{
    std::array<bool, 6> opened;
    opened[0] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX + 1, y + chunkY, z + chunkZ).id).opened_faces[adjacent(0)];
    opened[1] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX - 1, y + chunkY, z + chunkZ).id).opened_faces[adjacent(1)];
    opened[2] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY + 1, z + chunkZ).id).opened_faces[adjacent(2)];
    opened[3] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY - 1, z + chunkZ).id).opened_faces[adjacent(3)];
    opened[4] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY, z + chunkZ + 1).id).opened_faces[adjacent(4)];
    opened[5] = Block::getBlockByVoxelId(Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY, z + chunkZ - 1).id).opened_faces[adjacent(5)];

    return opened;
}

int ChunkMeshBuilder::adjacent(int face)
{
    if (face % 2 == 0)
        return face + 1;
    return face - 1;
}

void ChunkMeshBuilder::vertex(float x, float y, float z, float u, float v)
{
    vertices.push_back(_x + x);
    vertices.push_back(_y + y);
    vertices.push_back(_z + z);
    vertices.push_back(u);
    vertices.push_back(v);
    vertices.push_back(pack_to_float(calculate_light(), _face));
}

void ChunkMeshBuilder::index(uint a, uint b, uint c, uint d, uint e, uint f)
{
    indices.push_back(indexOffset + a);
    indices.push_back(indexOffset + b);
    indices.push_back(indexOffset + c);
    indices.push_back(indexOffset + d);
    indices.push_back(indexOffset + e);
    indices.push_back(indexOffset + f);
    indexOffset += 4;
}

float ChunkMeshBuilder::calculate_light(){
    light l = chunk->lightmap.get(_x, _y, _z);
    // int wx = chunkX + _x;
    // int wy = chunkY + _y;
    // int wz = chunkZ + _z;

    // float faceFactor = 0.2f;
    // float [] faceDarkening = [0.2f, 0.7f, 0.0f, 0.8f, 0.3f, 0.5f];

    // switch(_face){
    //     case 0:
    //         l = Engine::VoxelStorage->get_light(wx + 1, wy, wz);
    //         break;
    //     case 1:
    //         l = Engine::VoxelStorage->get_light(wx - 1, wy, wz);
    //         break;
    //     case 2:
    //         l = Engine::VoxelStorage->get_light(wx, wy + 1, wz);
    //         break;
    //     case 3:
    //         l = Engine::VoxelStorage->get_light(wx, wy - 1, wz);
    //         break;
    //     case 4:
    //         l = Engine::VoxelStorage->get_light(wx, wy, wz + 1);
    //         break;
    //     case 5:
    //         l = Engine::VoxelStorage->get_light(wx, wy, wz - 1);
    //         break;
    // }
    // light = light + faceDarkening[_face] * faceFactor;
    // if (light < 0.0f) light = 0.0f;
    // if (light > 1.0f) light = 1.0f;

    // switch(_face) {
    //     case 0:
    // }
    return pack_to_float(l.value, 0);
}