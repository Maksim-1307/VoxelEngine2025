#include "ChunksController.hpp"
#include "src/Engine.hpp"
#include "src/logic/Settings.hpp"

void ChunksController::update() {
    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Y = std::floor((float)this->camera->position.y / CHUNK_H);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W); // fix

    glm::ivec3 currentPos(X, Y, Z);
    glm::ivec3 delta = currentPos - this->camPos;

    if (delta.x != 0 || delta.y != 0 || delta.z != 0) { 
        
        Engine::pChunkMap->translate(delta.x, delta.y, delta.z); 

        this->camPos = currentPos;
        load_around(camPos);
        
        std::cout << "Moved to: " << camPos.x << ", " << camPos.y << ", " << camPos.z << "\n"; // fix 
    }
}

void ChunksController::handle_at(int x, int y, int z) {
    Chunk* chunk = Engine::pChunkMap->get(x, y, z);
    if (!chunk) return;
    // try {
    //     // Engine::pLighting->prebuildSkyLight(chunk);
    //     Engine::pLighting->buildSkyLight(x, z);
    // } catch (...) {
    //     std::cerr << "Failed to build light of chunk " << "\n";
    // }
    // if (chunk->state < STRUCTURES_GENERATED) {
    //     Engine::pGenerator->generate_ambient(x, z);
    // } 
    
    try {
        if (Settings::HARD_LOADING || !chunk->renderer || chunk->state <= VISIBLE) {

            if (chunk->state == MODIFIED) Engine::pLighting->prebuildSkyLight(chunk);

            if (Settings::RECURSIVE_LIGHTING) {
                Engine::pLighting->onChunkLoaded(chunk->X, chunk->Y, chunk->Z, true);
                Engine::pLighting->buildSkyLight(chunk->X, chunk->Y, chunk->Z);
            }

            sptr<Mesh> mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
            if (!mesh) return;
            
            if (chunk->renderer) chunk->renderer = nullptr;
            chunk->renderer = make_uptr<MeshRenderer>(std::move(mesh), MeshType::MESH3D);
            chunk->renderer->transform = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(chunk->X * CHUNK_W, chunk->Y * CHUNK_H, chunk->Z * CHUNK_W)
            );
            
        } else {
            // std::shared_ptr<Mesh> mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
            // if (!mesh) return;
            
            // if (chunk->renderer) delete(chunk->renderer);
            // chunk->renderer = new MeshRenderer(mesh, MeshType::MESH3D);
            chunk->renderer->transform = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(chunk->X * CHUNK_W, 0, chunk->Z * CHUNK_W)
            );
        }
        chunk->state = VISIBLE;
    } catch (...) {
        std::cerr << "Failed to build mesh for chunk at " 
                  << x << ", " << z << "\n";
    }
}

void ChunksController::load_around(glm::ivec3 center) {

    int size = Engine::pChunkMap->size;
    int distance = Settings::LOAD_DISTANCE;

    for (Chunk* chunk : Engine::pChunkMap->padding_chunks(1)) {
        if (chunk->state < STRUCTURES_GENERATED) {
            // Engine::pGenerator->generate_ambient(chunk->X, chunk->Z);
        } 
    }
    for (Chunk* chunk : Engine::pChunkMap->padding_chunks(2)) {
        if (chunk->state == STRUCTURES_GENERATED) {
            Engine::pLighting->prebuildSkyLight(chunk);
        } else {
            // Engine::pGenerator->generate_ambient(chunk->X, chunk->Z);
            Engine::pLighting->prebuildSkyLight(chunk);
        }
    }

    if (firstLoad) {
        for (int padding = 3; padding <= distance; padding++) {
            for (Chunk* chunk : Engine::pChunkMap->padding_chunks(padding)) {
                if (chunk->state == TERRAIN_GENERATED) { 
                    // Engine::pGenerator->generate_ambient(chunk->X, chunk->Z);
                }
                Engine::pLighting->prebuildSkyLight(chunk);
            }
        }
    }
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {
        if (chunk->state >= STRUCTURES_GENERATED || true) {
            handle_at(chunk->X, chunk->Y, chunk->Z);
        } 
    }
    firstLoad = false;
}

void ChunksController::draw_chunks() {
    // glm::ivec2 center = this->camPos;
    int distance = Settings::LOAD_DISTANCE;
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {
        if (chunk->state < VISIBLE) 
            handle_at(chunk->X, chunk->Y, chunk->Z);
        if (!chunk || !chunk->renderer || chunk->state < VISIBLE) continue;
        Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
        chunk->renderer->draw();
    }
    
    // for (int x = center.x - distance; x <= center.x + distance; x++) {
    //     for (int z = center.y - distance; z <= center.y + distance; z++) { // fix
    //         Chunk* chunk = Engine::pChunkMap->get(x, z);
    //         if (!chunk || !chunk->renderer || chunk->state < VISIBLE) continue;
            
    //         Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
    //         chunk->renderer->draw();
    //     }
    // }
}