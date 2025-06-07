#pragma once

#include <cstdint>

// 11 bit -> 2 bytes voxel
#define ID_SIZE 8
#define STATE_SIZE 3

struct voxel
{
    uint8_t id : ID_SIZE = 0;
    uint8_t state : STATE_SIZE = 0;
};
