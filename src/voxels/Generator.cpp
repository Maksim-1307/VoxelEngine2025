#include "Generator.hpp"
#include "src/Engine.hpp"

#include <cmath>
#include <algorithm>

#define STB_PERLIN_IMPLEMENTATION
#include "include/stb_perlin.h" 

#define SIZE_FACTOR 15.0f

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
    int X = x * CHUNK_W;
    int Y = y;
    int Z = z * CHUNK_W;

    Chunk *chunk = new Chunk();
    chunk->X = x;
    chunk->Y = y;
    chunk->Z = z;

    for (int lx = 0; lx < CHUNK_W; lx++){
        for (int lz = 0; lz < CHUNK_W; lz++){
            float gx = static_cast<float>(lx + X);
            float gz = static_cast<float>(lz + Z);

            // Large continental landmass shape
            float continent = stb_perlin_noise3_seed(
                gx / 280.0f, 0.0f, gz / 280.0f, 0, 0, 0, this->seed
            );

            // Domain warping — perturb coordinates with lower-frequency noise
            // to break up grid-aligned artifacts and create organic boundaries
            float warpX = stb_perlin_noise3_seed(
                gx / 90.0f, 0.0f, gz / 90.0f, 0, 0, 0, this->seed + 101
            ) * 30.0f;
            float warpZ = stb_perlin_noise3_seed(
                gx / 90.0f + 500.0f, 0.0f, gz / 90.0f + 500.0f, 0, 0, 0, this->seed + 102
            ) * 30.0f;

            // Terrain type: 0 = flat lowlands, 1 = rugged highlands
            // Domain-warped so regions have organic, non-grid shapes
            float ruggedness = stb_perlin_noise3_seed(
                (gx + warpX) / 75.0f, 0.0f, (gz + warpZ) / 75.0f, 0, 0, 0, this->seed + 103
            );
            ruggedness = ruggedness * 0.5f + 0.5f;

            // Base elevation from continent
            float baseHeight = continent * 20.0f + 30.0f;

            // Ridged noise — narrow, steep mountain ridges
            float ridgeNoise = stb_perlin_noise3_seed(
                (gx + warpX * 0.4f) / 16.0f, 0.0f, (gz + warpZ * 0.4f) / 16.0f,
                0, 0, 0, this->seed + 104
            );
            float ridge = 1.0f - std::abs(ridgeNoise);
            ridge = ridge * ridge;
            float mountainAdd = ridge * 32.0f * ruggedness;

            // Medium hills
            float detail = stb_perlin_noise3_seed(
                (gx + warpX * 0.6f) / 22.0f, 0.0f, (gz + warpZ * 0.6f) / 22.0f,
                0, 0, 0, this->seed + 105
            );
            float hillAdd = detail * 3.0f * (1.0f + ruggedness);

            // Subtle surface texture
            float micro = stb_perlin_noise3_seed(
                gx / 7.0f, 0.0f, gz / 7.0f, 0, 0, 0, this->seed + 106
            ) * 0.6f;

            // Flatten lowland areas (future water zones)
            float highT = std::clamp((continent + 0.3f) / 0.6f, 0.0f, 1.0f);
            float height = baseHeight * (0.7f + 0.3f * highT)
                         + (mountainAdd + hillAdd + micro) * (0.2f + 0.8f * highT);

            // Lake basins — depressions in low-lying areas
            float basinNoise = stb_perlin_noise3_seed(
                (gx + warpX * 0.7f) / 40.0f, 0.0f, (gz + warpZ * 0.7f) / 40.0f,
                0, 0, 0, this->seed + 107
            );
            float basinDepth = 1.0f - (basinNoise * 0.5f + 0.5f);
            basinDepth = basinDepth * basinDepth;
            float lowFactor = 1.0f - std::clamp((height - 25.0f) / 10.0f, 0.0f, 1.0f);
            height -= basinDepth * 8.0f * lowFactor;

            int finalHeight = static_cast<int>(height);
            if (finalHeight >= CHUNK_H) finalHeight = CHUNK_H - 1;
            if (finalHeight < 0) finalHeight = 0;

            int dirtBlocks = 3;
            if (finalHeight > 35) {
                dirtBlocks = 3 - (finalHeight - 20) / 3;
                if (dirtBlocks < 0) dirtBlocks = 0;
            }

            if (finalHeight <= 15) {
                for (int ly = finalHeight; ly >= finalHeight - dirtBlocks && ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {12, 0});
                }
                for (int ly = finalHeight - dirtBlocks - 1; ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {2, 0});
                }
            } else if (dirtBlocks > 0) {
                chunk->set_voxel(lx, finalHeight, lz, {3, 0});
                for (int ly = finalHeight - 1; ly >= finalHeight - dirtBlocks && ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {1, 0});
                }
                for (int ly = finalHeight - dirtBlocks - 1; ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {2, 0});
                }
            } else if (finalHeight <= 40) {
                chunk->set_voxel(lx, finalHeight, lz, {3, 0});
                for (int ly = finalHeight - 1; ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {2, 0});
                }
            } else {
                for (int ly = finalHeight; ly >= 0; ly--) {
                    chunk->set_voxel(lx, ly, lz, {2, 0});
                }
            }
        }
    }

    for (int lx = 0; lx < CHUNK_W; lx++){
        for (int ly = 0; ly < CHUNK_H; ly++){
            for (int lz = 0; lz < CHUNK_W; lz++){
                float perlin = stb_perlin_noise3_seed(
                    (lx + X) / SIZE_FACTOR, (ly + Y) / SIZE_FACTOR * 2, (lz + Z) / SIZE_FACTOR,
                    0, 0, 0, this->seed
                );
                if (perlin > 0.4f){
                    chunk->set_voxel(lx, ly, lz, {0, 0});
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