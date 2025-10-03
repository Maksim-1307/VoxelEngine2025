#pragma once

#include <glm/glm.hpp>

class VoxelStorage;

struct RaycastResult {
    bool hit;
    glm::vec3 position;
    float distance;
};


/* physical wrapper for AreaMap3D */

class Terrain {
public:
    Terrain(VoxelStorage& voxelStorage) : voxelStorage(voxelStorage) {
        
    }
    bool is_obstacle_at(float x, float y, float z);
    bool is_opaque_at(float x, float y, float z);
    RaycastResult raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, bool inside = true);

private:
    VoxelStorage& voxelStorage;
};