#include "ChunksController.hpp"
#include "src/Engine.hpp"

void ChunksController::update() {
    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Y = std::floor((float)this->camera->position.y / CHUNK_H);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W);

    glm::ivec3 currentPos(X, Y, Z);
    glm::ivec3 delta = currentPos - this->camPos;

    if (delta.x != 0 || delta.y != 0 || delta.z != 0) {

        Engine::pChunkMap->translate(delta.x, delta.y, delta.z);
        this->camPos = currentPos;
        load_around(camPos);
        
        std::cout << "Moved to: " << camPos.x << ", " << camPos.y << ", " << camPos.z << "\n";
    }
}

void ChunksController::handle_at(int x, int y, int z) {
    Chunk* chunk = Engine::pChunkMap->get(x, y, z);
    if (!chunk || chunk->renderer != nullptr) return;
    
    try {
        Mesh* mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
        if (!mesh) return;
        
        chunk->renderer = new MeshRenderer(mesh, MeshType::MESH3D);
        chunk->renderer->transform = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(chunk->X * CHUNK_W, chunk->Y * CHUNK_H, chunk->Z * CHUNK_W)
        );
    } catch (...) {
        std::cerr << "Failed to build mesh for chunk at " 
                  << x << ", " << y << ", " << z << "\n";
    }
}

void ChunksController::load_around(glm::ivec3 center) {

    for (int x = center.x - distance; x <= center.x + distance; x++) {
        for (int y = center.y - distance; y <= center.y + distance; y++) {
            for (int z = center.z - distance; z <= center.z + distance; z++) {
                handle_at(x, y, z);
            }
        }
    }
}

void ChunksController::draw_chunks() {

    glm::ivec3 center = this->camPos;
    
    for (int x = center.x - distance; x <= center.x + distance; x++) {
        for (int y = center.y - distance; y <= center.y + distance; y++) {
            for (int z = center.z - distance; z <= center.z + distance; z++) {
                Chunk* chunk = Engine::pChunkMap->get(x, y, z);
                if (!chunk || !chunk->renderer) continue;
                
                Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
                chunk->renderer->draw();
            }
        }
    }
}
