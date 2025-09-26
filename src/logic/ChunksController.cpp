#include "ChunksController.hpp"
#include "src/Engine.hpp"

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
    Chunk* chunk = Engine::pChunkMap->get(x, z);
    if (!chunk) return;
    try {
        Engine::pLighting->prebuildSkyLight(chunk);
    } catch (...) {
        std::cerr << "Failed to build light of chunk " << "\n";
    }
    
    try {
        Mesh* mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
        if (!mesh) return;
        
        if (chunk->renderer) delete(chunk->renderer);
        chunk->renderer = new MeshRenderer(mesh, MeshType::MESH3D);
        chunk->renderer->transform = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(chunk->X * CHUNK_W, 0, chunk->Z * CHUNK_W)
        );
        chunk->modified = false;
    } catch (...) {
        std::cerr << "Failed to build mesh for chunk at " 
                  << x << ", " << z << "\n";
    }
}

void ChunksController::load_around(glm::ivec2 center) {

    for (int x = center.x - distance; x <= center.x + distance; x++) {    
        for (int z = center.y - distance; z <= center.y + distance; z++) { // fix
                handle_at(x, z);
        }
    }
}

void ChunksController::draw_chunks() {

    glm::ivec2 center = this->camPos;
    
    for (int x = center.x - distance; x <= center.x + distance; x++) {
        for (int z = center.y - distance; z <= center.y + distance; z++) { // fix
            Chunk* chunk = Engine::pChunkMap->get(x, z);
            if (!chunk || !chunk->renderer) continue;
            
            Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
            chunk->renderer->draw();
        }
    }
}
