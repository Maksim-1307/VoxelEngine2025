#pragma once

#include <iostream>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Camera.hpp"

enum ChunksUpdatePhase {
    GENERATE_TERRAIN = 0,
    WAIT_FOR_TERRAIN = 1,
    GENERATE_AMBIENT = 2,
    WAIT_FOR_AMBIENT = 3,
    UPDATE_PRE_LIGHTING = 4,
    WAIT_FOR_PRE_LIGHTING = 5,
    UPDATE_LIGHTING = 6,
    WAIT_FOR_LIGHTING = 7,
    UPDATE_MESH = 8,
    WAIT_FOR_MESH = 9
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
        ChunksController(AreaMap2D *chunkMap, Camera *camera) : chunkMap(chunkMap), camera(camera) {
            chunkMap->fill();
            this->update();
            this->load_around(this->camPos);
        };
        void update();
        void set_distance(int distance);
        void draw_chunks();
        std::vector<sptr<Chunk>> get_loaded_chunks();
    private:
        void load_around(glm::ivec2 center);
        bool firstLoad = true;

        std::queue<sptr<Chunk>> handleQueue;
        ChunksUpdatePhase updatingPhase = ChunksUpdatePhase::GENERATE_TERRAIN;
        std::atomic<int> processingChunksCount{0};
        AreaMap2D *chunkMap;
        Camera* camera;
        glm::ivec2 camPos;
        
        inline ChunksUpdatePhase next_phase (ChunksUpdatePhase phase) const {
            switch (phase) {
                case ChunksUpdatePhase::GENERATE_TERRAIN:
                    return ChunksUpdatePhase::WAIT_FOR_TERRAIN;
                case ChunksUpdatePhase::WAIT_FOR_TERRAIN:
                    return ChunksUpdatePhase::GENERATE_AMBIENT;
                case ChunksUpdatePhase::GENERATE_AMBIENT:
                    return ChunksUpdatePhase::WAIT_FOR_AMBIENT;
                case ChunksUpdatePhase::WAIT_FOR_AMBIENT:
                    return ChunksUpdatePhase::UPDATE_PRE_LIGHTING;
                case ChunksUpdatePhase::UPDATE_PRE_LIGHTING:
                    return ChunksUpdatePhase::WAIT_FOR_PRE_LIGHTING;
                case ChunksUpdatePhase::WAIT_FOR_PRE_LIGHTING:
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

