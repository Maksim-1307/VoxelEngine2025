#include "WorldLoadingIndicator.hpp"

WorldLoadingIndicator::WorldLoadingIndicator(Array2D<Chunk*>* chunks) {
    mesh = std::make_shared<Mesh>(vertices.data(), vertices.size(), indices.data(), indices.size());
    renderer = new MeshRenderer(mesh, MeshType::SPRITE2D);
    this->chunks = chunks;
    
    update(this->chunks);
}

unsigned char* generateCheckerboardData(int width, int height) {
    // Выделяем память для данных (3 байта на пиксель: RGB)
    unsigned char* data = new unsigned char[width * height * 3];
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            
            if ((x + y) % 2 == 0) {
                // Красный пиксель: R=255, G=0, B=0
                data[index] = 255;     // R
                data[index + 1] = 0;   // G
                data[index + 2] = 0;   // B
            } else {
                // Зеленый пиксель: R=0, G=255, B=0
                data[index] = 0;       // R
                data[index + 1] = 255; // G
                data[index + 2] = 0;   // B
            }
        }
    }
    
    return data;
}


void WorldLoadingIndicator::update(Array2D<Chunk*>* chunks) {

    this->chunks = chunks;
    int width = chunks->getSizeX();
    int height = chunks->getSizeY();
    
    textureData.clear();
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Chunk* chunk = chunks->get(x, y);
            
            if (chunk == nullptr) {
                textureData.push_back(255); // R
                textureData.push_back(0);   // G  
                textureData.push_back(0);   // B
                continue;
            }
            
            ChunkState state = chunk->state;
            
            switch (state) {
                case VISIBLE:
                    textureData.push_back(255); textureData.push_back(255); textureData.push_back(255);
                    break;
                case LIGHTS_BUILT:
                    textureData.push_back(152); textureData.push_back(221); textureData.push_back(255);
                    break;
                case CHUNK_LOADED:
                    textureData.push_back(59); textureData.push_back(190); textureData.push_back(255);
                    break;
                case LIGHTS_PRE_BUILT:
                    textureData.push_back(0); textureData.push_back(86); textureData.push_back(130);
                    break;
                case MODIFIED:
                    textureData.push_back(0); textureData.push_back(255); textureData.push_back(255);
                    break;
                case STRUCTURES_GENERATED:
                    textureData.push_back(24); textureData.push_back(234); textureData.push_back(24);
                    break;
                case TERRAIN_GENERATED:
                    textureData.push_back(6); textureData.push_back(64); textureData.push_back(46);
                    break;
                case INITIALIZED:
                    textureData.push_back(0); textureData.push_back(0); textureData.push_back(0);
                    break;
                default:
                    textureData.push_back(0); textureData.push_back(0); textureData.push_back(0);
                    break;
            }
        }
    }
    
    delete texture;
    texture = new Texture(textureData.data(), width, height);
}


void WorldLoadingIndicator::draw() {
    renderer->draw();
}

unsigned char* WorldLoadingIndicator::get_data(){
    return nullptr;
}
