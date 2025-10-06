#pragma once

#include <iostream>

#include "src/logic/pointers.hpp"
#include "src/graphics/Texture.hpp"
#include "src/graphics/Mesh.hpp"

class MeshRenderer;

class BlockIcon {
public:
    BlockIcon(int blockId, int size = 128);
    BlockIcon(){};
    ~BlockIcon(){};

    void draw();
    Texture* getTexture();

private:
    std::vector<GLfloat> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
    };
    std::vector<GLuint> indices = {
        0, 1, 2,
        2, 3, 0
    };
    void make_texture(int size);
    sptr<Mesh> mesh;
    MeshRenderer* renderer;
    Texture* texture;
    int blockId;
};