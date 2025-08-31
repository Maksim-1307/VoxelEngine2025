#include "Generator.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h" 

#define FACTOR 25.0f

Chunk *Generator::generate_at(int x, int y, int z)
{
    // global coords
    int X = x * CHUNK_W;
    int Y = y * CHUNK_H;
    int Z = z * CHUNK_W;

    Chunk *chunk = new Chunk();
    chunk->X = x;
    chunk->Y = y;
    chunk->Z = z;
    for (int x = 0; x < CHUNK_W; x++){
        for (int y = 0; y < CHUNK_H; y++){
            for (int z = 0; z < CHUNK_W; z++){
                float perlin = stb_perlin_noise3_seed((x + X) / FACTOR, (y + Y) / FACTOR * 2, (z + Z) / FACTOR, 0, 0, 0, this->seed);
                if (perlin > 0.4f){
                    chunk->set_voxel(x, y, z, {1, 0});
                } else {
                    chunk->set_voxel(x, y, z, {0, 0});
                }
            }
        }
    }
    chunk->modified = true;
    chunk->lightmap.mask.clear(true);
    return chunk;
}