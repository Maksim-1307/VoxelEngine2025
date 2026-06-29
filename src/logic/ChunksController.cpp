#include "ChunksController.hpp"
#include "src/Engine.hpp"
#include "src/logic/Settings.hpp"

void ChunksController::update() {
    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W); // fix

    glm::ivec2 currentPos(X, Z);
    glm::ivec2 delta = currentPos - this->camPos;

    if (delta.x != 0 || delta.y != 0) { // fix
        
        Engine::pChunkMap->translate(delta.x, delta.y); // fix 

        this->camPos = currentPos;
        load_around(camPos);
        
        std::cout << "Moved to: " << camPos.x << ", " << camPos.y << "\n"; // fix 
    }
}

void ChunksController::handle_at(int x, int z) {

    Profiler t("handle_at");

    Chunk* chunk = Engine::pChunkMap->get(x, z);
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

        // building or updating sky light if its needed
        if (Settings::HARD_LOADING || !chunk->renderer || chunk->state < VISIBLE) {

            if (chunk->state == MODIFIED) Engine::pLighting->prebuildSkyLight(chunk);

            if (Settings::RECURSIVE_LIGHTING) {
                Engine::pLighting->onChunkLoaded(chunk->X, chunk->Z, true);
                Engine::pLighting->buildSkyLight(chunk->X, chunk->Z);
            }

            sptr<Mesh> mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
            if (!mesh) return;
            
            if (chunk->renderer) chunk->renderer = nullptr;
            chunk->renderer = make_uptr<MeshRenderer>(std::move(mesh), MeshType::MESH3D);
            chunk->renderer->transform = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(chunk->X * CHUNK_W, 0, chunk->Z * CHUNK_W)
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
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {
        if (chunk->state >= STRUCTURES_GENERATED || true) {
            handle_at(chunk->X, chunk->Z);
        } 
    }
    firstLoad = false;
}

void ChunksController::draw_chunks() {
    // glm::ivec2 center = this->camPos;
    int distance = Settings::LOAD_DISTANCE;
    for (Chunk* chunk : Engine::pChunkMap->chunks_in_radius(distance-1)) {
        if (chunk->state < VISIBLE) 
            handle_at(chunk->X, chunk->Z);
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