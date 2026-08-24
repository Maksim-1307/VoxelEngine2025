#include "ChunkMeshBuilder.hpp"
#include "src/Engine.hpp"

sptr<Mesh> ChunkMeshBuilder::buildMesh(Chunk &chunk)
{

    Profiler t("buildMesh");

    vertices.clear();
    indices.clear();
    vertices.reserve(5000);
    indices.reserve(2000);

    this->chunk = &chunk;
    int X = this->chunk->X;
    int Y = this->chunk->Y;
    int Z = this->chunk->Z;

    Chunk* nxChunk = Engine::pChunkMap->get(X-1, Z);
    Chunk* pxChunk = Engine::pChunkMap->get(X+1, Z);
    Chunk* nzChunk = Engine::pChunkMap->get(X, Z-1);
    Chunk* pzChunk = Engine::pChunkMap->get(X, Z+1);

    if (!nxChunk || !pxChunk || !nzChunk || !pzChunk) {
        throw std::runtime_error(
            "ChunkMeshBuilder::buildMesh: failed to build mesh for chunk at " + 
            std::to_string(X) + ", " + std::to_string(Z) + 
            " because neighbour chunk is not generated yet\n"
        );
    }

    // std::scoped_lock lock(
    //     nxChunk->mtx, pxChunk->mtx,
    //     nzChunk->mtx, pzChunk->mtx
    // );

    // caching
    this->currD = chunk.get_raw_data();
    this->nxD = nxChunk->get_raw_data();
    this->pxD = pxChunk->get_raw_data();
    this->nzD = nzChunk->get_raw_data();
    this->pzD = pzChunk->get_raw_data();

    indexOffset = 0;

    this->chunkX = X * CHUNK_W;
    this->chunkY = Y * CHUNK_H;
    this->chunkZ = Z * CHUNK_W;

    for (_x = 0; _x < CHUNK_W; _x++)
    {
        for (_y = 0; _y < CHUNK_H; _y++)
        {
            for (_z = 0; _z < CHUNK_W; _z++)
            {
                // Air always has id == 0
                if (get_voxel_fast(_x, _y, _z).id == 0) continue;
                Block& block = Block::getBlockByVoxelId(get_voxel_fast(_x, _y, _z).id);
                switch (block.getBlockModel()) {
                    case BlockModel::SOLID:
                    case BlockModel::FOLIAGE:
                        CubeModel(_x, _y, _z);
                        break;
                    case BlockModel::GRASS:
                        GrassModel(_x, _y, _z);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return  make_sptr<Mesh>(std::move(vertices), std::move(indices));
}

void ChunkMeshBuilder::CubeModel(int x, int y, int z)
{
    std::array<bool, 6> openedFaces = opened_around(x, y, z);
    Block& block = Block::getBlockByVoxelId(get_voxel_fast(x, y, z).id);

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

            switch (face)
            {
            case 0: case 3: case 4:
                index(3, 1, 0, 3, 2, 1);
                break;
            case 1: case 2: case 5:
                index(0, 1, 3, 1, 2, 3);
                break;
            }
        }
    }
}

void ChunkMeshBuilder::GrassModel(int x, int y, int z) 
{
    std::array<bool, 6> openedFaces = opened_around(x, y, z);
    Block& block = Block::getBlockByVoxelId(get_voxel_fast(x, y, z).id);

    bool isOpened = std::any_of(openedFaces.begin(), openedFaces.end(), [](bool b) { return b; });
    if (!isOpened) return;

    size_t UVx = std::get<0>(block.getUV(0));
    size_t UVy = std::get<1>(block.getUV(0));
    glm::vec2 uv = glm::vec2((float)UVx / ATLAS_SIZE, (float)UVy / ATLAS_SIZE);

    // Pseudo-random offset from block X Y Z so grass isn't always centered
    int wx = x + chunkX, wy = y + chunkY, wz = z + chunkZ;
    uint32_t h = (uint32_t)(wx * 73856093) ^ (uint32_t)(wy * 19349663) ^ (uint32_t)(wz * 83492791);
    float offX = ((h & 0xFF) / 255.0f - 0.5f) * 0.3f;
    float offZ = (((h >> 8) & 0xFF) / 255.0f - 0.5f) * 0.3f;

    // delta = 0.5 - 0.5 * cos(45 degrees)
    const float d = 0.146f;

    _face = 6; // Grass model doesnt have faces. Use inner light calculation for all vertices

    // First sprite (front face)
    vertex(d + offX, 0.0f, d + offZ, uv.x, uv.y);
    vertex(d + offX, 1.0f, d + offZ, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 1.0f, 1.0f-d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 0.0f, 1.0f-d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y);

    index(0, 1, 3, 1, 2, 3);

    // First sprite (back face)
    vertex(d + offX, 0.0f, d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y);
    vertex(d + offX, 1.0f, d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 1.0f, 1.0f-d + offZ, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 0.0f, 1.0f-d + offZ, uv.x, uv.y);

    index(3, 1, 0, 3, 2, 1);

    // Second sprite (front face)
    vertex(d + offX, 0.0f, 1.0f-d + offZ, uv.x, uv.y);
    vertex(d + offX, 1.0f, 1.0f-d + offZ, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 1.0f, d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 0.0f, d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y);

    index(0, 1, 3, 1, 2, 3);

    // Second sprite (back face)
    vertex(d + offX, 0.0f, 1.0f-d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y);
    vertex(d + offX, 1.0f, 1.0f-d + offZ, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 1.0f, d + offZ, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d + offX, 0.0f, d + offZ, uv.x, uv.y);

    index(3, 1, 0, 3, 2, 1);
}

std::array<bool, 6> ChunkMeshBuilder::opened_around(int x, int y, int z)
{
    std::array<bool, 6> opened{};
    opened[0] = Block::getBlockByVoxelId(get_voxel_fast(x + 1, y, z).id).opened_faces[adjacent(0)];
    opened[1] = Block::getBlockByVoxelId(get_voxel_fast(x - 1, y, z).id).opened_faces[adjacent(1)];
    opened[2] = Block::getBlockByVoxelId(get_voxel_fast(x, y + 1, z).id).opened_faces[adjacent(2)];
    opened[3] = Block::getBlockByVoxelId(get_voxel_fast(x, y - 1, z).id).opened_faces[adjacent(3)];
    opened[4] = Block::getBlockByVoxelId(get_voxel_fast(x, y, z + 1).id).opened_faces[adjacent(4)];
    opened[5] = Block::getBlockByVoxelId(get_voxel_fast(x, y, z - 1).id).opened_faces[adjacent(5)];

    return opened;
}

inline voxel ChunkMeshBuilder::get_voxel_fast(int x, int y, int z) {

    if (is_in_bounds(x, y, z)) return currD[GET_VOXEL_INDEX(x, y, z)];
    if (x == -1) return nxD[GET_VOXEL_INDEX(CHUNK_W-1, y, z)];
    if (x == CHUNK_W) return pxD[GET_VOXEL_INDEX(0, y, z)];
    if (y == -1 || y == CHUNK_H) return {0, 0}; // air 
    if (z == -1) return nzD[GET_VOXEL_INDEX(x, y, CHUNK_W-1)];
    if (z == CHUNK_W) return pzD[GET_VOXEL_INDEX(x, y, 0)];

    std::cout << "WARNING: Bad position in ChunkMeshBuilder::get_voxel_fast. Worst case called" << std::endl;
    std::cout << x << " " << y << " " << z << "\n";
    return Engine::pVoxelStorage->get_voxel(x + chunkX, y + chunkY, z + chunkZ);
}

// not fast yet 
inline light ChunkMeshBuilder::get_light_fast(int x, int y, int z) {
    return Engine::pVoxelStorage->get_light(x + chunkX, y + chunkY, z + chunkZ);
}

inline bool ChunkMeshBuilder::is_in_bounds(int x, int y, int z) {
    return x >= 0 && x < CHUNK_W && y >= 0 && y < CHUNK_H && z >= 0 && z < CHUNK_W;
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

uint16_t ChunkMeshBuilder::calculate_light(){
    const int coords[] = {
        1, 0, 0, // x+
       -1, 0, 0, // x-
        0, 1, 0, // y+
        0,-1, 0, // ...
        0, 0, 1,
        0, 0,-1,
        0, 0, 0  // inside
    };
    int face = _face;
    int x = _x + coords[face * 3 + 0];
    int y = _y + coords[face * 3 + 1];
    int z = _z + coords[face * 3 + 2];

    return get_light_fast(x, y, z).value;

}
