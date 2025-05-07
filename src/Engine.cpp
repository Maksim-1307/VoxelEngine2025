#include <iostream>
#include <chrono>

#include "Engine.hpp"

using namespace std::chrono;

Engine::Engine(){}

Window* Engine::pWindow = nullptr;
Shader *Engine::pSpriteShader = nullptr;
Mesh* Engine::pMesh = nullptr;
MeshRenderer *Engine::pRenderer = nullptr;

void Engine::init() {
    WindowArgs wargs;
    wargs.name = "VoxelEngine";
    Engine::pWindow = new Window(wargs);

    // TEST
    Engine::pSpriteShader = nullptr;
    Engine::pSpriteShader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
    // why segfault??
    Engine::pMesh = new Mesh();
    Engine::pMesh->vertices = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    Engine::pMesh->indices = {
        0, 1, 2,
        1, 3, 2};
    Engine::pRenderer = new MeshRenderer(Engine::pMesh, MeshType::SPRITE2D);
    // TEST
}

void Engine::game_loop(){

    std::cout << "game loop started\n";
    auto lastTime = high_resolution_clock::now();
    while(!Engine::pWindow->should_close()){

        auto currentTime = high_resolution_clock::now();
        float deltaTime = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        Engine::pSpriteShader->use();
        Engine::pRenderer->draw();


        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
    }
    std::cout << "game loop interrupted";

}