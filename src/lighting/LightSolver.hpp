#pragma once

#include <queue>

#include "light.hpp"
#include "src/logic/AreaMap3D.hpp"
#include "src/voxels/Chunk.hpp"

// class AreaMap3D;
// class Chunk;

struct lightentry {
    int x;
    int y;
    int z;
    unsigned char light;
};

class LightSolver {
    std::queue<lightentry> addqueue;
    std::queue<lightentry> remqueue;
    AreaMap3D<Chunk>& chunks;
    int channel;
public:
    LightSolver(AreaMap3D<Chunk>& chunks, int channel);

    void add(int x, int y, int z);
    void add(int x, int y, int z, unsigned char emission);
    void remove(int x, int y, int z);
    void solve();
};
