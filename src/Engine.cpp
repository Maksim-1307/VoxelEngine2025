#include <iostream>
#include <chrono>

#include "Engine.hpp"

#include "graphics/Mesh.hpp"
#include "graphics/MeshRenderer.hpp"
#include "graphics/Shader.hpp"

using namespace std::chrono;

Engine::Engine(){}

Window* Engine::pWindow = nullptr;

void Engine::init() {
    WindowArgs wargs;
    wargs.name = "VoxelEngine";
    //wargs.clearColor = glm::vec3(0.5f, 0.5f, 1.0f);
    Engine::pWindow = new Window(wargs);
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


        // TEST

        Shader shader("res/shaders/shader.vert",
                      "res/shaders/shader.frag");

        shader.use();

        Mesh mesh;
        mesh.vertices = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
        mesh.indices = {
            0, 1, 2,
            1, 3, 2};

        MeshRenderer renderer(&mesh);
        renderer.gen_buffers(MeshType::MESH3D);
        renderer.draw();
        // TEST


        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
    }
    std::cout << "game loop interrupted";

}