#pragma once

#include <iostream>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Camera.hpp"

enum ChunksUpdatePhase {
    GENERATE_TERRAIN = 0,
    WAIT_FOR_TERRAIN = 1,
    UPDATE_LIGHTING = 2,
    WAIT_FOR_LIGHTING = 3,
    UPDATE_MESH = 4,
    WAIT_FOR_MESH = 5
};

enum ChunkUpdateOperation {
    TERRAIN_GENERATION,
    STRUCTURES_GENERATION,
    LIGHTS_PRE_BUILDING,
    LIGHTS_BUILDING,
    MESH_BUILDING
};

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
        void load_around(glm::ivec2 center);
        bool firstLoad = true;

        std::queue<Chunk*> handleQueue;
        ChunksUpdatePhase updatingPhase = ChunksUpdatePhase::GENERATE_TERRAIN;
        std::atomic<int> processingChunksCount{0};
        AreaMap2D<Chunk> *chunkMap;
        Camera* camera;
        glm::ivec2 camPos;
        
        inline ChunksUpdatePhase next_phase (ChunksUpdatePhase phase) const {
            switch (phase) {
                case ChunksUpdatePhase::GENERATE_TERRAIN:
                    return ChunksUpdatePhase::WAIT_FOR_TERRAIN;
                case ChunksUpdatePhase::WAIT_FOR_TERRAIN:
                    return ChunksUpdatePhase::UPDATE_LIGHTING;
                case ChunksUpdatePhase::UPDATE_LIGHTING:
                    return ChunksUpdatePhase::WAIT_FOR_LIGHTING;
                case ChunksUpdatePhase::WAIT_FOR_LIGHTING:
                    return ChunksUpdatePhase::UPDATE_MESH;
                case ChunksUpdatePhase::UPDATE_MESH:
                    return ChunksUpdatePhase::WAIT_FOR_MESH;
                case ChunksUpdatePhase::WAIT_FOR_MESH:
                    return ChunksUpdatePhase::GENERATE_TERRAIN;
                default:
                    std::cout << "ERROR: unknown chunk update phase in ChunksController::update(). Numeric value : " << (int)updatingPhase << std::endl;
                    return ChunksUpdatePhase::GENERATE_TERRAIN;
            }
        }

        bool canPerformFor(ChunkPos pos, ChunkUpdateOperation operation);
};

