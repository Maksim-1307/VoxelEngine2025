#include "Generator.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h" 

#define SIZE_FACTOR 15.0f
#define BASE_HEIGHT 25
#define CAVE_FACTOR 0.3f

Chunk *Generator::generate_at(int x, int z)
{
    return terrain_with_caves(x, 0, z);
}

Chunk* Generator::terrain_with_caves(int x, int y, int z){
    // global coords
    int X = x * CHUNK_W;
    int Y = y;
    int Z = z * CHUNK_W;

    Chunk *chunk = new Chunk();
    chunk->X = x;
    chunk->Y = y;
    chunk->Z = z;
    // column generation 
    for (int x = 0; x < CHUNK_W; x++){
        for (int z = 0; z < CHUNK_W; z++){
            float perlinTerrain = stb_perlin_noise3_seed((x + X) / SIZE_FACTOR, 0.0f, (z + Z) / SIZE_FACTOR, 0, 0, 0, this->seed);
            int height = (int)(perlinTerrain * 10 + 15);
            if (height >= CHUNK_H) height = CHUNK_H-1;
            int y = height;

            chunk->set_voxel(x, y, z, {3, 0});
            y--;
            for (; y >= height-3 && y >= 0; y--){
                chunk->set_voxel(x, y, z, {1, 0});                
            }
            for (; y >= 0; y--) {
                chunk->set_voxel(x, y, z, {2, 0});
            }
        }
    }
    // caves generation
    for (int x = 0; x < CHUNK_W; x++){
        for (int y = 0; y < CHUNK_H; y++){
            for (int z = 0; z < CHUNK_W; z++){
                float perlin = stb_perlin_noise3_seed((x + X) / SIZE_FACTOR, (y + Y) / SIZE_FACTOR * 2, (z + Z) / SIZE_FACTOR, 0, 0, 0, this->seed);
                if (perlin > 0.4f){
                    chunk->set_voxel(x, y, z, {0, 0});
                }
            }
        }
    }

    return chunk;
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