#pragma once

#include <iostream>
#include "src/logic/AreaMap2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Camera.hpp"

enum ChunksUpdatePhase {
    UPDATE_LIGHTING,
    WAIT_FOR_LIGHTING,
    UPDATE_MESH,
    WAIT_FOR_MESH
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
        ChunksUpdatePhase updatingPhase = ChunksUpdatePhase::UPDATE_LIGHTING;
        std::atomic<int> processingChunksCount{0};
        AreaMap2D<Chunk> *chunkMap;
        Camera* camera;
        glm::ivec2 camPos;
        
        inline ChunksUpdatePhase next_phase (ChunksUpdatePhase phase) const {
            switch (phase) {
                case ChunksUpdatePhase::UPDATE_LIGHTING:
                    return ChunksUpdatePhase::WAIT_FOR_LIGHTING;
                case ChunksUpdatePhase::WAIT_FOR_LIGHTING:
                    return ChunksUpdatePhase::UPDATE_MESH;
                case ChunksUpdatePhase::UPDATE_MESH:
                    return ChunksUpdatePhase::WAIT_FOR_MESH;
                case ChunksUpdatePhase::WAIT_FOR_MESH:
                    return ChunksUpdatePhase::UPDATE_LIGHTING;
                default:
                    std::cout << "ERROR: unknown chunk update phase in ChunksController::update(). Numeric value : " << (int)updatingPhase << std::endl;
                    return ChunksUpdatePhase::UPDATE_LIGHTING;
            }
        }

        bool canPerformFor(ChunkPos pos, ChunkUpdateOperation operation);
};

