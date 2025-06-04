#include <iostream>
#include <chrono>

#include "Engine.hpp"

using namespace std::chrono;

Engine::Engine() {}

Window *Engine::pWindow = nullptr;
Shader *Engine::pSpriteShader = nullptr;
Shader *Engine::pMeshShader = nullptr;
Mesh *Engine::pMesh = nullptr;
Mesh *Engine::pSprite = nullptr;
MeshRenderer *Engine::pRenderer = nullptr;
MeshRenderer *Engine::pSpriteRenderer = nullptr;
Camera *Engine::pCamera = nullptr;
CameraController *Engine::pCameraController = nullptr;
Canvas *Engine::pCanvas = nullptr;
Texture *Engine::pTexture = nullptr;

void Engine::init()
{
    WindowArgs wargs;
    wargs.name = "VoxelEngine";
    Engine::pWindow = new Window(wargs);

    Engine::pSpriteShader = new Shader("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    Engine::pMeshShader = new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag");

    Engine::pMesh = new Mesh();
    Engine::pMesh->vertices = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };
    Engine::pMesh->indices = {
        2, 1, 0,
        2, 3, 1};

    Engine::pSprite = new Mesh();
    Engine::pSprite->vertices = {
        0.0f, 0.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 100.0f, 0.0f, 1.0f,
        100.0f, 100.0f, 1.0f, 1.0f};
    Engine::pSprite->indices = {
        2, 1, 0,
        2, 3, 1};

    Engine::pTexture = new Texture("res/textures/test.jpg");
    Engine::pRenderer = new MeshRenderer(Engine::pMesh, MeshType::MESH3D);
    Engine::pSpriteRenderer = new MeshRenderer(Engine::pSprite, MeshType::SPRITE2D);
    Engine::pCamera = new Camera(*Engine::pWindow);
    Engine::pCameraController = new CameraController(*Engine::pCamera, *Engine::pWindow);
    Engine::pCanvas = new Canvas(*Engine::pWindow);
}

void Engine::game_loop()
{

    FPSCounter* fpsCounter = new FPSCounter();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    std::cout << "game loop started\n";
    auto lastTime = high_resolution_clock::now();

    while (!Engine::pWindow->should_close())
    {

        auto currentTime = high_resolution_clock::now();
        float deltaTime = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fpsCounter->update(deltaTime);
        Engine::pCameraController->update(deltaTime);

        // rendering a mesh
        Engine::pMeshShader->use();

        view = pCamera->getView();
        projection = pCamera->getProjection();

        Engine::pMeshShader->set_matrix4("model", model);
        Engine::pMeshShader->set_matrix4("view", view);
        Engine::pMeshShader->set_matrix4("projection", projection);
        Engine::pMeshShader->set_texture("theTexture", Engine::pTexture->getID());
        Engine::pRenderer->draw();

        // rendering a sprite
        Engine::pSpriteShader->use();

        projection = pCanvas->get_projection();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 10.0f, 0.0f));

        Engine::pSpriteShader->set_matrix4("projection", projection * transform);
        Engine::pSpriteShader->set_texture("theTexture", Engine::pTexture->getID());
        Engine::pSpriteRenderer->draw();

        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
        glfwPollEvents();
    }
    std::cout << "game loop interrupted";
}