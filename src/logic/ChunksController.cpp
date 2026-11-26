#include "ChunksController.hpp"
#include "src/Engine.hpp"
#include "src/logic/Settings.hpp"
#include "ChunksUpdater.hpp"
#include "src/logic/ThreadPool.hpp"

void ChunksController::update() {

    // Moving world's center while player's moving
    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W);

    glm::ivec2 currentPos(X, Z);
    glm::ivec2 delta = currentPos - this->camPos;

    if (delta.x != 0 || delta.y != 0 || firstLoad) {
        
        Engine::pChunkMap->translate(delta.x, delta.y);
        this->camPos = currentPos;
        load_around(camPos);
        
        std::cout << "Moved to: " << camPos.x << ", " << camPos.y << "\n";
    }

    // === Chunks updating logic ===

    // Updating chunks updating phase
    switch (updatingPhase) {
        case ChunksUpdatePhase::WAIT_FOR_TERRAIN:
        case ChunksUpdatePhase::WAIT_FOR_AMBIENT:
        case ChunksUpdatePhase::WAIT_FOR_PRE_LIGHTING:
        case ChunksUpdatePhase::WAIT_FOR_MESH:
            // If all chunks already processed - moving to the next phase
            if (processingChunksCount == 0) {
                updatingPhase = next_phase(updatingPhase);
                // std::cout << "Next phase: " << (int)updatingPhase << std::endl;
            } else if (processingChunksCount < 0) {
                std::cout << "ERROR: negative value of processingChunksCount" << std::endl;
            } else {
                // std::cout << "count: " << processingChunksCount << std::endl;
            }
            break;
        case ChunksUpdatePhase::WAIT_FOR_LIGHTING:
            break;
        case ChunksUpdatePhase::GENERATE_TERRAIN:
        case ChunksUpdatePhase::GENERATE_AMBIENT:
        case ChunksUpdatePhase::UPDATE_PRE_LIGHTING:
        case ChunksUpdatePhase::UPDATE_LIGHTING:
        case ChunksUpdatePhase::UPDATE_MESH:
            break;
        default:
            std::cout << "ERROR: unknown chunk update phase in ChunksController::update(). Numeric value : " << (int)updatingPhase << std::endl;
            break;
    }

    int distance = Settings::LOAD_DISTANCE;

    // Zero phase (generation)
    if (updatingPhase == ChunksUpdatePhase::GENERATE_TERRAIN) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance+2)) { // Implement iterator for all chunks
            if (chunk->state < TERRAIN_GENERATED) {
                ChunkPos chunkPos = {chunk->X, chunk->Z};
                if (!canPerformFor(chunkPos, TERRAIN_GENERATION)) continue;
                processingChunksCount += 1;
                ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
                    ChunksUpdater::get_instance().generate_terrain(chunkPos);
                    // Decrement counter once the chunk is processed
                    processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
                });
            }
        }
        updatingPhase = ChunksUpdatePhase::WAIT_FOR_TERRAIN;
    }

    // Ambient generation phase
    if (updatingPhase == ChunksUpdatePhase::GENERATE_AMBIENT) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance+2)) { // Implement iterator for all chunks
            if (chunk->state < STRUCTURES_GENERATED) {
                ChunkPos chunkPos = {chunk->X, chunk->Z};
                if (!canPerformFor(chunkPos, STRUCTURES_GENERATION)) continue;
                processingChunksCount += 1;
                ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
                    ChunksUpdater::get_instance().generate_ambient(chunkPos);
                    // Decrement counter once the chunk is processed
                    processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
                });
            }
        }
        updatingPhase = ChunksUpdatePhase::WAIT_FOR_AMBIENT;
    }

    // First phase (Pre-Lighting)
    if (updatingPhase == ChunksUpdatePhase::UPDATE_PRE_LIGHTING) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance+2)) {
            if (chunk->state < LIGHTS_PRE_BUILT) {
                ChunkPos chunkPos = {chunk->X, chunk->Z};
                if (!canPerformFor(chunkPos, LIGHTS_PRE_BUILDING)) continue;
                processingChunksCount += 1;
                ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
                    ChunksUpdater::get_instance().update_pre_light(chunkPos);
                    // Decrement counter once the chunk is processed
                    processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
                });
            }
        }
        updatingPhase = ChunksUpdatePhase::WAIT_FOR_PRE_LIGHTING;
    }

    // Second phase (Lighting)
    if (updatingPhase == ChunksUpdatePhase::UPDATE_LIGHTING || updatingPhase == ChunksUpdatePhase::WAIT_FOR_LIGHTING) {
        if (processingChunksCount == 0) {
            for (sptr<Chunk> chunk : Engine::pChunkMap->gridTraversing(phase_bias)) {
                if (chunk->state < LIGHTS_BUILT) {
                    ChunkPos chunkPos = {chunk->X, chunk->Z};
                    if (!canPerformFor(chunkPos, LIGHTS_BUILDING)) continue;
                    processingChunksCount += 1;
                    ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
                        ChunksUpdater::get_instance().update_light(chunkPos);
                        // Decrement counter once the chunk is processed
                        processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
                    });
                }
            }
            phase_bias += 1;
            if (phase_bias > 8) {
                updatingPhase = UPDATE_MESH;
                phase_bias = 0;
            }
        } else if (processingChunksCount < 0) {
            std::cout << "ERROR: negative value of processingChunksCount" << std::endl;
        }

        // for (sptr<Chunk> chunk : Engine::pChunkMap->gridTraversing(0)) {
        //     if (chunk->state < LIGHTS_BUILT) {
        //         ChunkPos chunkPos = {chunk->X, chunk->Z};
        //         if (!canPerformFor(chunkPos, LIGHTS_BUILDING)) continue;
        //         processingChunksCount += 1;
        //         ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
        //             ChunksUpdater::get_instance().update_light(chunkPos);
        //             // Decrement counter once the chunk is processed
        //             processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
        //         });
        //     }
        // }
        // updatingPhase = ChunksUpdatePhase::WAIT_FOR_LIGHTING;
    }

    // Last phase (Mesh building)
    if (updatingPhase == ChunksUpdatePhase::UPDATE_MESH) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance+2)) {
            if (chunk->state < MESH_BUILT && chunk->state >= LIGHTS_BUILT) {
                ChunkPos chunkPos = {chunk->X, chunk->Z};
                if (!canPerformFor(chunkPos, MESH_BUILDING)) continue;
                processingChunksCount += 1;
                ThreadPool::get_instance().enqueue([chunkPos, this, &processingChunksCount = processingChunksCount]() {
                    ChunksUpdater::get_instance().update_mesh(chunkPos);
                    // Decrement counter once the chunk is processed
                    processingChunksCount.fetch_sub(1, std::memory_order_relaxed);
                });
            }
        }
        updatingPhase = ChunksUpdatePhase::WAIT_FOR_MESH;
    }

}

void ChunksController::load_around(glm::ivec2 center) {
    firstLoad = false;
    return;

    int size = Engine::pChunkMap->size;
    int distance = Settings::LOAD_DISTANCE;

    for (int p = 1; p <= distance; p++) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->padding_chunks(p)) {
            if (chunk->state < STRUCTURES_GENERATED) {
                Engine::pGenerator->generate_ambient(chunk->X, chunk->Z);
            }
        }
    }
    for (int p = 2; p <= distance; p++) {
        for (sptr<Chunk> chunk : Engine::pChunkMap->padding_chunks(p)) {
            if (chunk->state < LIGHTS_PRE_BUILT) {
                Engine::pLighting->prebuildSkyLight(chunk);
            }
        }
    }
    // padding() only returns ring perimeters, so the exact center chunk is
    // never prepared above. Handle it explicitly, otherwise its worker task
    // exits early on the state guard and it never becomes VISIBLE.
    if (sptr<Chunk> centerChunk = Engine::pChunkMap->get(center.x, center.y)) {
        if (centerChunk->state < STRUCTURES_GENERATED) {
            Engine::pGenerator->generate_ambient(centerChunk->X, centerChunk->Z);
        }
        if (centerChunk->state < LIGHTS_PRE_BUILT) {
            Engine::pLighting->prebuildSkyLight(centerChunk);
        }
    }
    for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance-2)) {
        if (chunk->state < VISIBLE) {
        } 
    }
    firstLoad = false;
}

void ChunksController::draw_chunks() {
    // glm::ivec2 center = this->camPos;
    int distance = Settings::LOAD_DISTANCE;
    for (sptr<Chunk> chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {

        if (chunk->pendingMesh) {
            chunk->renderer = make_uptr<MeshRenderer>(std::move(chunk->pendingMesh), MeshType::MESH3D);
            chunk->renderer->transform = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(chunk->X * CHUNK_W, 0, chunk->Z * CHUNK_W)
            );
            chunk->state = VISIBLE;
        }

        if (chunk->state == MESH_BUILT) continue;

        if (chunk->renderer) {
            Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
            chunk->renderer->draw();
        }
    }
}

std::vector<sptr<Chunk>> ChunksController::get_loaded_chunks() {
    return Engine::pChunkMap->chunks_in_radius(Settings::LOAD_DISTANCE-1);
}

// Can some update operation be performed on chunk considering its neighbors states
// Ensures correctness for operation
bool ChunksController::canPerformFor(ChunkPos pos, ChunkUpdateOperation operation) {
    int x = pos.x;
    int z = pos.z;
    sptr<Chunk> chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return false;
    if (operation == TERRAIN_GENERATION && chunk->state == INITIALIZED) return true;
    if (operation == MESH_BUILDING && chunk->state == LIGHTS_BUILT) return true;
    ChunkState min_neighbors_state = VISIBLE;
    std::array<sptr<Chunk>, 8> neighbors;
    int i = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            if (dx == 0 && dz == 0) continue;
            sptr<Chunk> n = Engine::pChunkMap->get(x + dx, z + dz);
            if (!n) return false;
            neighbors[i++] = n;
            if (n->state < min_neighbors_state) min_neighbors_state = n->state;
        }
    }
    switch (operation) {
        case TERRAIN_GENERATION:
            // Don't generate chunk unless it's empty 
            return chunk->state == INITIALIZED;
        case STRUCTURES_GENERATION:
            // Don't generate structures if at least one neighbor or itself is not terrain generated yet 
            return (chunk->state >= TERRAIN_GENERATED) && (min_neighbors_state >= TERRAIN_GENERATED);
        case LIGHTS_PRE_BUILDING:
            // Don't pre-build light if at least one neighbor or itself is not structures generated yet
            return (chunk->state >= STRUCTURES_GENERATED) && (min_neighbors_state >= STRUCTURES_GENERATED);
        case LIGHTS_BUILDING:
            // Don't build light without pre-building for neighbors end itself
            return (chunk->state >= LIGHTS_PRE_BUILT) && (min_neighbors_state >= LIGHTS_PRE_BUILT);
        case MESH_BUILDING:
            return (chunk->state >= LIGHTS_BUILT) && (min_neighbors_state >= LIGHTS_BUILT);
        default:
            std::cout << "ERROR: unknown chunk update operation in ChunksController::canPerformFor(). Numeric value : " << (int)updatingPhase << std::endl;
            return false;
    }
    return false;
}