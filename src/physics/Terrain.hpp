#pragma once

class VoxelStorage;

class Terrain {
public:
    Terrain(VoxelStorage& voxelStorage) : voxelStorage(voxelStorage) {
        
    }
    bool is_obstacle_at(float x, float y, float z);

private:
    VoxelStorage& voxelStorage;
};