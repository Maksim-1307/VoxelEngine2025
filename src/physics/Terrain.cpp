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


// RaycastResult Terrain::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance) {

//     bool prevObstacle = is_obstacle_at(origin.x, origin.y, origin.z);
//     if (prevObstacle) return {
//         true,
//         origin,
//         0
//     };
//     const float stepLen = 0.1f;
//     const float error = 0.01f;
//     glm::vec3 step = glm::normalize(direction) * stepLen;
//     glm::vec3 current = origin;
//     float distance = 0.0f;
//     int i = 0;

//     while (glm::length(step) > error && !is_obstacle_at(current.x, current.y, current.z)) {
//         i++;
//         current += step;
//         distance = glm::length(current - origin);
//         if (distance > maxDistance) return {
//             false,
//             glm::vec3(0.0f),
//             distance
//         };
//         bool obstacle = is_obstacle_at(current.x, current.y, current.z);
//         if (prevObstacle != obstacle) step /= -2;
//         prevObstacle = obstacle;
//     }

//     std::cout << "Raycast completed in " << i << " steps." << std::endl;
//     return {
//         true,
//         current,
//         distance
//     };
// }

