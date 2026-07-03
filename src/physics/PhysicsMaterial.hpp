#pragma once

// Defines physics behaviour while colliding or intersecting with voxels
// The higher the value, the higher the priority
enum class PhysicsMaterial {
    FREE = 0,
    LEAVES,
    WATER,
    SOLID,
};