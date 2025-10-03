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
    // voxel vox = voxelStorage.get_voxel(vx, vy, vz);
    std::vector<AABB> AABBs = Block::getAABBs(vx, vy, vz);
    for (AABB hitbox : AABBs){
        if (hitbox.contains(glm::vec3(ix, iy, iz))) return true;
    }
    return false;
}

/*

public Voxel? RayCast(Vector3 pos, Vector3 dir, float maxDist){
        Vector3 step = 0.1f * dir;
        Vector3 current = pos;
        while ((pos - current).Length < maxDist) {
            current += step;
            if (IsObstableAt(current)) {
                int x = (int)MathF.Floor(current.X);
                int y = (int)MathF.Floor(current.Y);
                int z = (int)MathF.Floor(current.Z);
                SetVoxel(x, y, z, new Voxel(0,0));
                return _voxelStorage.GetVoxel(x, y, z);
            }
        }
        return null;
    }


*/


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

RaycastResult Terrain::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance) {

    glm::vec3 step = 0.1f * direction;
    glm::vec3 current = origin;
    while (glm::length(current - origin) < maxDistance) {
        current += step;
        if (is_obstacle_at(current.x, current.y, current.z)) {
            return{
                true,
                current + 0.05f * direction,
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