#pragma once

#include <queue>

#include "light.hpp"
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"

struct lightentry {
    int x;
    int y;
    int z;
    light value;
};

class LightSolver {
    std::queue<lightentry> addqueue;
    std::queue<lightentry> remqueue;
    AreaMap2D<Chunk>& chunks;
public:
    LightSolver(AreaMap2D<Chunk>& chunks);

    void add(int x, int y, int z);
    void add(int x, int y, int z, light value);
    void remove(int x, int y, int z);
    void solve();
};
