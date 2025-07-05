#pragma once

#define GLEW_STATIC
#define GLM_FORCE_CTOR_INIT

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"

enum class MeshType { MESH3D, SPRITE2D, LINE3D };

class MeshRenderer {
    public:
        MeshRenderer(Mesh* pMesh, MeshType type) {
            this->pMesh = pMesh;
            this->gen_buffers(type);
        };
        ~MeshRenderer(){
            clear_data();
        }
        void draw();
        glm::mat4 transform = glm::mat4(1.0f);

    private:
        void gen_buffers(MeshType type);
        void clear_data();
        Mesh* pMesh;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
};
