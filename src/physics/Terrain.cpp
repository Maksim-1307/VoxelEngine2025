#include "Terrain.hpp"
#include "AABB.hpp"
#include "src/voxels/VoxelStorage.hpp"
#include "src/voxels/Block.hpp"

bool Terrain::is_obstacle_at(float x, float y, float z) {
    int vx = std::floor(x);
    int vy = std::floor(y);
    int vz = std::floor(z);
    float ix = x - vx;
    float iy = y - vy;
    float iz = z - vz;
    voxel vox = voxelStorage.get_voxel(vx, vy, vz);
    std::vector<AABB> AABBs = Block::getAABBs(vox);
    for (AABB hitbox : AABBs){
        if (hitbox.contains(glm::vec3(ix, iy, iz))) return true;
    }
    return false;
}