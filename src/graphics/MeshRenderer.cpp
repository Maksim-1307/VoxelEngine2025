#include "MeshRenderer.hpp"

void MeshRenderer::gen_buffers(MeshType type){

    switch (type){
        case MeshType::MESH3D:
            /* layout: float pos X, float poz Y, float pos Z, float UV X, float UV Z */
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, pMesh->vertices.size() * sizeof(GLfloat), &pMesh->vertices[0], GL_STATIC_DRAW);

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->indices.size() * sizeof(GLuint), &pMesh->indices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;

        case MeshType::SPRITE2D:
            /* layout: float pos X, float poz Y, float UV X, float UV Z */
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, pMesh->vertices.size() * sizeof(GLfloat), &pMesh->vertices[0], GL_STATIC_DRAW);

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->indices.size() * sizeof(GLuint), &pMesh->indices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;
        case MeshType::LINE3D:
            std::cerr << "ERROR! Type LINE3D is not supported yet.";
            break;
    }
    

}

void MeshRenderer::draw(){

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, pMesh->vertices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}