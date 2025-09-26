#include "Generator.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h" 

#define SIZE_FACTOR 15.0f
#define BASE_HEIGHT 25

Chunk *Generator::generate_at(int x, int z)
{
    return perlin_noise_2d(x, 0, z);
}

Chunk* Generator::perlin_noise_3d(int x, int y, int z){
    // global coords
    int X = x * CHUNK_W;
    int Y = y;
    int Z = z * CHUNK_W;

    Chunk *chunk = new Chunk();
    chunk->X = x;
    chunk->Y = y;
    chunk->Z = z;
    for (int x = 0; x < CHUNK_W; x++){
        for (int y = 0; y < CHUNK_H; y++){
            for (int z = 0; z < CHUNK_W; z++){
                float perlin = stb_perlin_noise3_seed((x + X) / SIZE_FACTOR, (y + Y) / SIZE_FACTOR * 2, (z + Z) / SIZE_FACTOR, 0, 0, 0, this->seed);
                if (perlin > 0.4f){
                    chunk->set_voxel(x, y, z, {1, 0});
                } else {
                    chunk->set_voxel(x, y, z, {0, 0});
                }
            }
        }
    }
    // chunk->modified = true;
    // chunk->lightmap.mask.clear(true);
    return chunk;
}

Chunk* Generator::perlin_noise_2d(int x, int y, int z){
    // global coords
    int X = x * CHUNK_W;
    int Y = y * CHUNK_H;
    int Z = z * CHUNK_W;

    Chunk *chunk = new Chunk();
    chunk->X = x;
    chunk->Y = y;
    chunk->Z = z;
    for (int x = 0; x < CHUNK_W; x++){
        for (int z = 0; z < CHUNK_W; z++){
            for (int y = 0; y < CHUNK_H; y++){
            
                float perlin = stb_perlin_noise3_seed((x + X) / SIZE_FACTOR, 0.0f, (z + Z) / SIZE_FACTOR, 0, 0, 0, this->seed);
                float height = perlin * 10 + 15;
                if (Y + y > height){
                    chunk->set_voxel(x, y, z, {0, 0});
                } else {
                    chunk->set_voxel(x, y, z, {1, 0});
                }
            }
        }
    }
    // chunk->modified = true;
    // chunk->lightmap.mask.clear(true);
    return chunk;
}