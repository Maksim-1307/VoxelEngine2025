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
    std::vector<AABB> AABBs = Block::getAABBs(vx, vy, vz);
    for (AABB hitbox : AABBs){
        if (hitbox.contains(glm::vec3(ix, iy, iz))) return true;
    }
    return false;
}

bool Terrain::is_opaque_at(float x, float y, float z) {
    int vx = std::floor(x);
    int vy = std::floor(y);
    int vz = std::floor(z);
    float ix = x - vx;
    float iy = y - vy;
    float iz = z - vz;
    std::vector<AABB> AABBs = Block::getAABBs(vx, vy, vz, IteractionType::RAYCAST);
    for (AABB hitbox : AABBs){
        if (hitbox.contains(glm::vec3(ix, iy, iz))) return true;
    }
    return false;
}

PhysicsMaterial Terrain::get_material_at(float x, float y, float z) {
    int vx = std::floor(x);
    int vy = std::floor(y);
    int vz = std::floor(z);
    float ix = x - vx;
    float iy = y - vy;
    float iz = z - vz;
    std::vector<AABB> AABBs = Block::getAABBs(vx, vy, vz);
    PhysicsMaterial material = PhysicsMaterial::FREE;
    for (AABB hitbox : AABBs){
        if (hitbox.contains(glm::vec3(ix, iy, iz))) {
            material = std::max(material, hitbox.material);
        }
    }
    return material;
}

RaycastResult Terrain::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, bool inside) {

    glm::vec3 step = 0.1f * direction;
    glm::vec3 current = origin;
    while (glm::length(current - origin) < maxDistance) {
        current += step;
        if (is_opaque_at(current.x, current.y, current.z)) {
            if (inside) {
                current += 0.05f * direction;
            } else {
                current -= step;
            }
            return{
                true,
                current,
                glm::length(current - origin)
            };
        }
    }
    return {
        false,
        glm::vec3(0.0f),
        0
    };
}