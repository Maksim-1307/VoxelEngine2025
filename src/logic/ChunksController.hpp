#pragma once

#include <iostream>
#include "src/logic/AreaMap3D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Camera.hpp"
/*

заполнять AreaMap
сдвигать AreaMap при движении камеры

*/
class ChunksController {
    public:
        ChunksController(AreaMap3D<Chunk> *chunkMap, Camera *camera) : chunkMap(chunkMap), camera(camera) {
            chunkMap->fill();
            this->update();
            this->load_around(this->camPos);
        };
        void update();
        void set_distance(int distance);
        void draw_chunks();
    private:
        void handle_at(int x, int y, int z);
        void load_around(glm::ivec3 center);
        int distance = 5;
        AreaMap3D<Chunk> *chunkMap;
        Camera* camera;
        glm::ivec3 camPos;
};

