#pragma once

#include "LightSolver.hpp"
#include "Lightmap.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/voxels/voxel.hpp"
#include "src/voxels/Block.hpp"
#include "src/logic/AreaMap2D.hpp"
#include <queue>


class Lighting {

    AreaMap2D<Chunk>& chunks;
    std::unique_ptr<LightSolver> solverR;
    std::unique_ptr<LightSolver> solverG;
    std::unique_ptr<LightSolver> solverB;
    std::unique_ptr<LightSolver> solverS;
public:
    Lighting(AreaMap2D<Chunk>& chunks);
    ~Lighting();

    void clear();
    void buildSkyLight(int cx, int cz);
    void onChunkLoaded(int cx, int cz, bool expand);
    void onBlockSet(int x, int y, int z, uint8_t id);
    void syncBorders(Chunk* main, int nX, int nZ, int side);

    static void prebuildSkyLight(Chunk* chunk);
private:
    static std::queue<Chunk*> preBuildQueue;
};
