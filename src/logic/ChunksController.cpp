#include "ChunksController.hpp"
#include "src/Engine.hpp"

void ChunksController::update(){

    int X = std::floor((float)this->camera->position.x / CHUNK_W);
    int Y = std::floor((float)this->camera->position.y / CHUNK_H);
    int Z = std::floor((float)this->camera->position.z / CHUNK_W);

    glm::ivec3 currentPos(X, Y, Z);

    if (currentPos != this->camPos) {
        glm::ivec3 delta = currentPos - this->camPos;
        this->camPos = currentPos;
        std::cout << "movement\n";
        Engine::pChunkMap->translate(delta.x, delta.y, delta.z);
    }
}

void ChunksController::handle_at(int x, int y, int z){
    Chunk* chunk = Engine::pChunkMap->get(x, y, z);
    if (chunk->renderer != nullptr) return;
    Mesh *mesh = Engine::pChunkMeshBuilder->buildMesh(*chunk);
    chunk->renderer = new MeshRenderer(mesh, MeshType::MESH3D);
    chunk->renderer->transform = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->X * CHUNK_W, chunk->Y * CHUNK_H, chunk->Z * CHUNK_W));
}

void ChunksController::load_around(glm::ivec3 center){
    for (int x = center.x - distance; x < center.x + distance; x++)
    {
        for (int y = center.y - distance; y < center.y + distance; y++)
        {
            for (int z = center.z - distance; z < center.z + distance; z++)
            {
                handle_at(x, y, z);
            }
        }
    }
}

// void ChunksController::draw_chunks(){
//     Chunk** chunks = Engine::pChunkMap->get_data();
//     int size = Engine::pChunkMap->get_size();
//     for (int i = 0; i < size*size*size; i++){
//         Chunk* chunk = chunks[i];
//         if (chunk->renderer == nullptr) {
//             // continue;
//             int x = chunk->X; 
//             int y = chunk->Y;
//             int z = chunk->Z;

//             if (Engine::pChunkMap->is_inside(x, y, z))
//             {
//                 handle_at(chunk->X, chunk->Y, chunk->Z);
//             } else {
//                 continue;
//             }
//         }
//         Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
//         chunk->renderer->draw();
//     }
// }

void ChunksController::draw_chunks()
{
    glm::ivec3 center = this->camPos;
    for (int x = center.x - distance; x < center.x + distance; x++)
    {
        for (int y = center.y - distance; y < center.y + distance; y++)
        {
            for (int z = center.z - distance; z < center.z + distance; z++)
            {
                Chunk *chunk = Engine::pChunkMap->get(x, y, z);
                if (chunk->renderer == nullptr)
                {
                    // continue;
                    handle_at(x, y, z);
                }
                Engine::pMeshShader->set_matrix4("model", chunk->renderer->transform);
                chunk->renderer->draw();
            }
        }
    }
    Chunk **chunks = Engine::pChunkMap->get_data();
    int size = Engine::pChunkMap->get_size();
    for (int i = 0; i < size * size * size; i++)
    {
        
    }
}