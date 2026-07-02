#pragma once

#include <iostream>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Camera.hpp"
/*

заполнять AreaMap
сдвигать AreaMap при движении камеры

*/
class ChunksController {
    public:
        ChunksController(AreaMap2D<Chunk> *chunkMap, Camera *camera) : chunkMap(chunkMap), camera(camera) {
            chunkMap->fill();
            this->update();
            this->load_around(this->camPos);
        };
        void update();
        void set_distance(int distance);
        void draw_chunks();
        std::vector<Chunk*> get_loaded_chunks();
    private:
        void handle_at(int x, int z);
        void load_around(glm::ivec2 center);
        bool firstLoad = true;

        std::queue<Chunk*> handleQueue;
        AreaMap2D<Chunk> *chunkMap;
        Camera* camera;
        glm::ivec2 camPos;
};

