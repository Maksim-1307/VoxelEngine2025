#include "WorldLoadingIndicator.hpp"

WorldLoadingIndicator::WorldLoadingIndicator(Array2D<Chunk*>* chunks) {
    mesh = make_sptr<Mesh>(std::move(vertices), std::move(indices));
    renderer = new MeshRenderer(std::move(mesh), MeshType::SPRITE2D);
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
                case VISIBLE: // White
                    textureData.push_back(255); textureData.push_back(255); textureData.push_back(255);
                    break;
                case LIGHTS_BUILT: // Turquoise
                    textureData.push_back(152); textureData.push_back(221); textureData.push_back(255);
                    break;
                case CHUNK_LOADED: // Light blue
                    textureData.push_back(59); textureData.push_back(190); textureData.push_back(255);
                    break;
                case LIGHTS_PRE_BUILT: // Dark blue
                    textureData.push_back(0); textureData.push_back(86); textureData.push_back(130);
                    break;
                case MODIFIED: // Green
                    textureData.push_back(0); textureData.push_back(255); textureData.push_back(255);
                    break;
                case STRUCTURES_GENERATED: // Light green
                    textureData.push_back(24); textureData.push_back(234); textureData.push_back(24);
                    break;
                case TERRAIN_GENERATED: // Dark green
                    textureData.push_back(6); textureData.push_back(64); textureData.push_back(46);
                    break;
                case MESH_BUILT: // Orange
                    textureData.push_back(238); textureData.push_back(191); textureData.push_back(1);
                    break;
                case INITIALIZED: // Black
                    textureData.push_back(0); textureData.push_back(0); textureData.push_back(0);
                    break;
                default: // Black
                    std::cout << "Unknown chunk state: " << static_cast<int>(state) << "\n";
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
