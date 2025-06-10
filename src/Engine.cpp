#include <iostream>
#include <chrono>

#include "Engine.hpp"

using namespace std::chrono;

Engine::Engine() {}

Window *Engine::pWindow = nullptr;
Shader *Engine::pSpriteShader = nullptr;
Shader *Engine::pTextShader = nullptr;
Shader *Engine::pMeshShader = nullptr;
Mesh *Engine::pMesh = nullptr;
Mesh *Engine::pSprite = nullptr;
MeshRenderer *Engine::pRenderer = nullptr;
MeshRenderer *Engine::pSpriteRenderer = nullptr;
Camera *Engine::pCamera = nullptr;
InputController *Engine::pInputController = nullptr;
Canvas *Engine::pCanvas = nullptr;
Texture *Engine::pTexture = nullptr;
Text *Engine::pText = nullptr;
Chunk *Engine::pChunk = nullptr;
ChunkMeshBuilder *Engine::pChunkMeshBuilder = nullptr;

void Engine::init()
{

    new Block("air", BlockModel::AIR,     {});
    new Block("dirt", BlockModel::SOLID,  {{0, 1}});
    new Block("stone", BlockModel::SOLID, {{1, 0}});
    new Block("grass", BlockModel::SOLID, {{1, 1}, {0, 0}, {0, 1}});
    new Block("oak_log", BlockModel::SOLID, {{2, 1}, {0, 2}, {2, 1}});
    new Block("leaves", BlockModel::SOLID, {{1, 2}});

    WindowArgs wargs;
    wargs.name = "VoxelEngine";
    Engine::pWindow = new Window(wargs);

    Engine::pSpriteShader = new Shader("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    Engine::pTextShader = new Shader("res/shaders/sprite.vert", "res/shaders/text.frag");
    Engine::pMeshShader = new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag");

    Engine::pMesh = new Mesh();
    // Engine::pMesh->vertices = {
    //     0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    //     0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    //     1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    // };
    // Engine::pMesh->indices = {
    //     2, 1, 0,
    //     2, 3, 1};

    Engine::pSprite = new Mesh();
    Engine::pSprite->vertices = {
        0.0f, 0.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 100.0f, 0.0f, 1.0f,
        100.0f, 100.0f, 1.0f, 1.0f};
    Engine::pSprite->indices = {
        2, 1, 0,
        2, 3, 1};

    Engine::pChunk = new Chunk();
    Engine::pChunk->set_voxel(2, 2, 2, {1, 0});
    Engine::pChunk->set_voxel(2, 3, 3, {1, 0});
    Engine::pChunk->set_voxel(3, 3, 3, {2, 0});
    Engine::pChunk->set_voxel(5, 3, 3, {3, 0});
    Engine::pChunk->set_voxel(7, 3, 3, {4, 0});
    Engine::pChunk->set_voxel(9, 3, 3, {5, 0});
    Engine::pChunkMeshBuilder = new ChunkMeshBuilder();

    Engine::pMesh = Engine::pChunkMeshBuilder->buildMesh(*Engine::pChunk);
    Engine::pTexture = new Texture("res/textures/atlas.png", true);
    Engine::pRenderer = new MeshRenderer(Engine::pMesh, MeshType::MESH3D);
    Engine::pSpriteRenderer = new MeshRenderer(Engine::pSprite, MeshType::SPRITE2D);
    Engine::pCamera = new Camera(*Engine::pWindow);
    Engine::pInputController = new InputController(*Engine::pCamera, *Engine::pWindow);
    Engine::pCanvas = new Canvas(*Engine::pWindow);
    std::string ss = "Day 5\nMaking a Minecraft clone on OpenGL";
    Engine::pText = new Text(ss);
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
        Engine::pInputController->update(deltaTime);
        //Engine::pText->update("Day 3\nMaking a Minecraft clone on OpenGL");

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
        Engine::pTextShader->use();

        projection = pCanvas->get_projection();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 60.0f, 0.0f));

        Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f)));
        Engine::pTextShader->set_texture("theTexture", Engine::pTexture->getID());
        // Engine::pSpriteRenderer->draw();

        Engine::pTextShader->set_texture("theTexture", Engine::pText->get_font()->get_texture()->getID());
        Engine::pText->draw();

        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
        glfwPollEvents();
    }
    std::cout << "game loop interrupted";
}