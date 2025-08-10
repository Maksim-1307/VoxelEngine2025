#pragma once

#include "LightSolver.hpp"
#include "Lightmap.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/voxels/voxel.hpp"
#include "src/voxels/Block.hpp"


class Lighting {

    AreaMap3D<Chunk>& chunks;
    std::unique_ptr<LightSolver> solverR;
    std::unique_ptr<LightSolver> solverG;
    std::unique_ptr<LightSolver> solverB;
    std::unique_ptr<LightSolver> solverS;
public:
    Lighting(AreaMap3D<Chunk>& chunks);
    ~Lighting();

    void clear();
    void buildSkyLight(int cx, int cy, int cz);
    void onChunkLoaded(int cx, int cy, int cz, bool expand);
    void onBlockSet(int x, int y, int z, uint8_t id);

    static void prebuildSkyLight(Chunk* chunk);
};
