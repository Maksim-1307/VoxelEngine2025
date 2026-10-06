#include "ChunksController.hpp"
#include "src/Engine.hpp"
#include "src/logic/Settings.hpp"
#include "ChunksUpdater.hpp"

void ChunksController::update() {
    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W);

    glm::ivec2 currentPos(X, Z);
    glm::ivec2 delta = currentPos - this->camPos;

    if (delta.x != 0 || delta.y != 0) {
        
        Engine::pChunkMap->translate(delta.x, delta.y);
        this->camPos = currentPos;
        load_around(camPos);
        
        std::cout << "Moved to: " << camPos.x << ", " << camPos.y << "\n";
    }

    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(Settings::LOAD_DISTANCE-2)) {
        // Re-queue modified chunks and prepared chunks whose worker task
        // never finished (e.g. it exited early), so they can't get stuck
        // before VISIBLE. MESH_BUILT chunks are left for draw_chunks.
        if (!chunk->isQueued &&
            (chunk->state == MODIFIED ||
             chunk->state == LIGHTS_PRE_BUILT ||
             chunk->state == LIGHTS_BUILT)) {
            ChunksUpdater::get_instance().queue_chunk({chunk->X, chunk->Z});
        }
    }
}

void ChunksController::handle_at(int x, int z) {

    Profiler t("handle_at");

    Chunk* chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return;
    
    try {

        if (!chunk->isQueued) {
            ChunksUpdater::get_instance().queue_chunk({x, z});
            chunk->isQueued = true;
        }
    } catch (...) {
        std::cerr << "Failed to build mesh for chunk at " 
                  << x << ", " << z << "\n";
    }
}

void ChunksController::load_around(glm::ivec2 center) {

    int size = Engine::pChunkMap->size;
    int distance = Settings::LOAD_DISTANCE;

    for (int p = 1; p <= distance; p++) {
        for (Chunk* chunk : Engine::pChunkMap->padding_chunks(p)) {
            if (chunk->state < STRUCTURES_GENERATED) {
                Engine::pGenerator->generate_ambient(chunk->X, chunk->Z);
            }
        }
    }
    for (int p = 2; p <= distance; p++) {
        for (Chunk* chunk : Engine::pChunkMap->padding_chunks(p)) {
            if (chunk->state < LIGHTS_PRE_BUILT) {
                Engine::pLighting->prebuildSkyLight(chunk);
            }
        }
    }
    // padding() only returns ring perimeters, so the exact center chunk is
    // never prepared above. Handle it explicitly, otherwise its worker task
    // exits early on the state guard and it never becomes VISIBLE.
    if (Chunk* centerChunk = Engine::pChunkMap->get(center.x, center.y)) {
        if (centerChunk->state < STRUCTURES_GENERATED) {
            Engine::pGenerator->generate_ambient(centerChunk->X, centerChunk->Z);
        }
        if (centerChunk->state < LIGHTS_PRE_BUILT) {
            Engine::pLighting->prebuildSkyLight(centerChunk);
        }
    }
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-2)) {
        if (chunk->state < VISIBLE) {
            handle_at(chunk->X, chunk->Z);
        } 
    }
    firstLoad = false;
}

void ChunksController::draw_chunks() {
    // glm::ivec2 center = this->camPos;
    int distance = Settings::LOAD_DISTANCE;
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {

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

std::vector<Chunk*> ChunksController::get_loaded_chunks() {
    return Engine::pChunkMap->chunks_in_radius(Settings::LOAD_DISTANCE-1);
}