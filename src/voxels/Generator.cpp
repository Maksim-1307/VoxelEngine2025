#include "Generator.hpp"
#include "src/Engine.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h" 

#define SIZE_FACTOR 15.0f
#define BASE_HEIGHT 25
#define CAVE_FACTOR 0.3f

Chunk *Generator::generate_at(int x, int z)
{
    Profiler t("generate_at");
    return terrain_with_caves(x, 0, z);
}

void Generator::generate_ambient(int x, int z) {
    Chunk* chunk = Engine::pChunkMap->get(x, z);

    if (chunk->state != TERRAIN_GENERATED) {
        std::cout << "WARNING: generate_ambient is called on chunk with a state other than TERRAIN_GENERATED\n";
        return;
    }

    int wx_start = chunk->X * CHUNK_W;
    int wz_start = chunk->Z * CHUNK_W;

    for (int wx = wx_start; wx < (x + 1) * CHUNK_W; wx++) {
        for (int wz = wz_start; wz < (z + 1) * CHUNK_W; wz++) {  
            int wy = CHUNK_H - 1;
            for (; wy >= 0; wy--) {
                if (Engine::pVoxelStorage->get_voxel(wx, wy, wz).id != 0) {
                    wy += 1;
                    break;
                }
            }
            float treeNoise = stb_perlin_noise3_seed(
                (x + X) / SIZE_FACTOR * 5, 
                0.0f, 
                (z + Z) / SIZE_FACTOR * 5, 
                0, 0, 0, this->seed
            );
            if (random(wx, wz, seed) > 0.98 && treeNoise > 0.015f) 
                generate_tree(wx, wy, wz);
            if (random(wx, wz, seed) > 0.9f)
                generate_grass(wx, wy, wz);
        }   
    }
    chunk->state = STRUCTURES_GENERATED;
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
            int height = (int)(perlinTerrain * 10 + 40);
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
    chunk->state = TERRAIN_GENERATED;
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
    chunk->state = TERRAIN_GENERATED;
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
                float height = perlin * 10 + 40;
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
    chunk->state = TERRAIN_GENERATED;
    return chunk;
}


void Generator::generate_tree(int x, int y, int z) {
    if (y == 0 || Engine::pVoxelStorage->get_voxel(x, y-1, z).id != 3) return;

    int trunk_height = 4 + (x % 3); 
    int crown_start_y = y + trunk_height;
    
    int crown_radius = 2;
    
    for (int dx = -crown_radius; dx <= crown_radius; dx++) {
        for (int dz = -crown_radius; dz <= crown_radius; dz++) {
            if (dx*dx + dz*dz <= crown_radius*crown_radius + 1) {
                Engine::pVoxelStorage->set_voxel(x + dx, crown_start_y, z + dz, {5, 0}); 
            }
        }
    }
    for (int dx = -crown_radius; dx <= crown_radius; dx++) {
        for (int dz = -crown_radius; dz <= crown_radius; dz++) {
            if (dx*dx + dz*dz <= crown_radius*crown_radius + 1) {
                Engine::pVoxelStorage->set_voxel(x + dx, crown_start_y-1, z + dz, {5, 0}); 
            }
        }
    }
    
    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            if (dx*dx + dz*dz <= 2) {
                Engine::pVoxelStorage->set_voxel(x + dx, crown_start_y + 1, z + dz, {5, 0}); 
            }
        }
    }

    for (int i = 0; i <= trunk_height; i++) {
        Engine::pVoxelStorage->set_voxel(x, y + i, z, {4, 0}); 
    }
    Engine::pVoxelStorage->set_voxel(x, y - 1, z, {1, 0}); 
    
    Engine::pVoxelStorage->set_voxel(x, crown_start_y + 2, z, {5, 0}); // Foliage
}

void Generator::generate_grass(int x, int y, int z) {
    if (y <= 0 || y >= CHUNK_H) return;
    voxel bottomVoxel = Engine::pVoxelStorage->get_voxel(x, y-1, z);
    voxel currentVoxel = Engine::pVoxelStorage->get_voxel(x, y, z);
    if (bottomVoxel.id != 3 && bottomVoxel.id != 1) return; // Only grass on grass or dirt
    if (currentVoxel.id != 0) return; // Grass can only be placed on empty space
    Engine::pVoxelStorage->set_voxel(x, y, z, {10, 0});  // Grass
}