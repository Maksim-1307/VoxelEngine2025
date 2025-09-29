#pragma once

#include <iostream>
#include "src/logic/Array2D.hpp"
#include "src/voxels/Chunk.hpp"
#include "src/graphics/Texture.hpp"

class WorldLoadingIndicator
{
public:
    WorldLoadingIndicator(Array2D<Chunk*>* chunks);
    void update();
    void draw();

    Texture* texture;

private:
    unsigned char* get_data();

    std::vector<GLfloat> vertices = {
        -1, -1, 0, 0,
        1, -1, 1, 0,
        -1, 1, 0, 1,
        1, 1, 1, 1
    };
    std::vector<GLuint> indices = {
        3, 1, 0,
        2, 3, 0
    };
    Mesh* mesh;
    MeshRenderer* renderer;
    Array2D<Chunk*>* chunks;
    std::vector<unsigned char> textureData;
};
