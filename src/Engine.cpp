#include <iostream>
#include <chrono>

#include "Engine.hpp"

using namespace std::chrono;

Engine::Engine(){}

Window*       Engine::pWindow = nullptr;
Shader*       Engine::pSpriteShader = nullptr;
Shader*       Engine::pMeshShader = nullptr;
Mesh*         Engine::pMesh = nullptr;
MeshRenderer* Engine::pRenderer = nullptr;
Camera*       Engine::pCamera = nullptr;
Canvas*       Engine::pCanvas = nullptr;
Texture*      Engine::pTexture = nullptr;

void Engine::init() {
    WindowArgs wargs;
    wargs.name = "VoxelEngine";
    Engine::pWindow = new Window(wargs);

    // TEST
    Engine::pSpriteShader = new Shader("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    Engine::pMeshShader = new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag");

    Engine::pMesh = new Mesh();
    Engine::pMesh->vertices = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  1.0f, 0.0f, 1.0f,
        1.0f,   1.0f, 1.0f, 1.0f,
         };
    Engine::pMesh->indices = {
        2, 1, 0,
        2, 3, 1};
    Engine::pTexture = new Texture("res/textures/test.jpg");
    Engine::pRenderer = new MeshRenderer(Engine::pMesh, MeshType::SPRITE2D);
    Engine::pCamera = new Camera(*Engine::pWindow);
    Engine::pCanvas = new Canvas(*Engine::pWindow);
    // TEST
}

void Engine::game_loop(){

    std::cout << "game loop started\n";
    auto lastTime = high_resolution_clock::now();
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.5, -0.1, 0.0f));
    while(!Engine::pWindow->should_close()){

        auto currentTime = high_resolution_clock::now();
        float deltaTime = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Engine::pSpriteShader->use();

        glm::mat4 projection = Engine::pCanvas->get_projection();
        glm::mat4 transform = Engine::pCanvas->get_positioning(POSITION::TOP, 0, POSITION::LEFT, 0);
        // transform = 

        Engine::pSpriteShader->set_matrix4("projection", projection * transform);
        Engine::pSpriteShader->set_texture("theTexture", Engine::pTexture->getID());
        Engine::pRenderer->draw();


        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
        glfwPollEvents();
    }
    std::cout << "game loop interrupted";

}