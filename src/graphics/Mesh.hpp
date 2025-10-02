#pragma once

#define GLEW_STATIC
#define GLM_FORCE_CTOR_INIT

#include <iostream>
#include <vector>
#include <GL/glew.h>


class Mesh
{
public:
    Mesh(std::vector<GLfloat>&& vertices, std::vector<GLuint>&& indices)
        : vertices(std::move(vertices)), indices(std::move(indices))
    {};
    ~Mesh() {};

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

