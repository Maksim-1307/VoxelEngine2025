#include <iostream>
#include <chrono>

#include "Engine.hpp"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

using namespace std::chrono;

Engine::Engine() {}

Window *Engine::pWindow = nullptr;
Shader *Engine::pSpriteShader = nullptr;
Shader *Engine::pTextShader = nullptr;
Shader *Engine::pMeshShader = nullptr;
MeshRenderer *Engine::pSpriteRenderer = nullptr;
Camera *Engine::pCamera = nullptr;
InputController *Engine::pInputController = nullptr;
Canvas *Engine::pCanvas = nullptr;
Texture *Engine::pTexture = nullptr;
Text *Engine::pText = nullptr;
Text *Engine::pFpsText = nullptr;
ChunkMeshBuilder *Engine::pChunkMeshBuilder = nullptr;
AreaMap3D<Chunk>* Engine::pChunkMap = nullptr;
Generator *Engine::pGenerator = nullptr;
VoxelStorage* Engine::pVoxelStorage = nullptr;
ChunksController* Engine::pChunksController = nullptr;
Terrain* Engine::pTerrain = nullptr;
Stats* Engine::pStats = nullptr;
Lighting* Engine::pLighting = nullptr;

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
    wargs.clearColor = {0.67f, 0.84f, 0.9f};
    Engine::pWindow = new Window(wargs);

    Engine::pSpriteShader = new Shader("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    Engine::pTextShader = new Shader("res/shaders/sprite.vert", "res/shaders/text.frag");
    Engine::pMeshShader = new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag");

    Engine::pChunkMeshBuilder = new ChunkMeshBuilder();
    Engine::pTexture = new Texture("res/textures/atlas.png", true);
    Engine::pCamera = new Camera(*Engine::pWindow);
    Engine::pInputController = new InputController(*Engine::pCamera, *Engine::pWindow);
    Engine::pCanvas = new Canvas(*Engine::pWindow);
    std::string ss = "Making a Minecraft clone on OpenGL";
    Engine::pText = new Text(ss);
    std::string sss = "fps: ";
    Engine::pFpsText = new Text(sss);
    Engine::pGenerator = new Generator();
    Engine::pChunkMap = new AreaMap3D<Chunk>(7);
    std::function<Chunk *(int, int, int)> gen_func = [](int x, int y, int z) -> Chunk* 
    { 
        return Engine::pGenerator->generate_at(x, y, z);
    };
    Engine::pChunkMap->set_out_callback(gen_func);
    Engine::pVoxelStorage = new VoxelStorage(Engine::pChunkMap);
    Engine::pChunksController = new ChunksController(Engine::pChunkMap, Engine::pCamera);
    Engine::pTerrain = new Terrain(*Engine::pVoxelStorage);
    Engine::pStats = new Stats();
    Engine::pLighting = new Lighting(*Engine::pChunkMap);
}

void Engine::game_loop()
{

    FPSCounter* fpsCounter = new FPSCounter();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    std::cout << "game loop started\n";
    auto lastTime = high_resolution_clock::now();
    bool prevObstacle = true;
    std::string s = "";

    while (!Engine::pWindow->should_close())
    {

        auto currentTime = high_resolution_clock::now();
        float deltaTime = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // updates
        fpsCounter->update(deltaTime);
        Engine::pInputController->update(deltaTime);
        Engine::pChunksController->update();

        // drawing terrain
        Engine::pMeshShader->use();

        view = pCamera->getView();
        projection = pCamera->getProjection();

        Engine::pMeshShader->set_matrix4("model", model);
        Engine::pMeshShader->set_matrix4("view", view);
        Engine::pMeshShader->set_matrix4("projection", projection);
        Engine::pMeshShader->set_texture("theTexture", Engine::pTexture->getID());

        Engine::pChunksController->draw_chunks();

        // drawing text
        Engine::pTextShader->use();

        projection = pCanvas->get_projection();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 60.0f, 0.0f));

        Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f)));
        Engine::pTextShader->set_texture("theTexture", Engine::pText->get_font()->get_texture()->getID());
        Engine::pText->draw();

        transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 120.0f, 0.0f));
        Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f)));

        glm::vec3 camPos = Engine::pCamera->position;
        bool obstacle = Engine::pTerrain->is_obstacle_at(camPos.x, camPos.y, camPos.z);
        Engine::pStats->set("Obstacle", obstacle ? "true" : "false");
        prevObstacle = obstacle;

        transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 250.0f, 0.0f));
        Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f)));
        Engine::pStats->draw();

        glfwSwapBuffers(Engine::pWindow->get_glfw_window());
        glfwPollEvents();
    }
    std::cout << "game loop interrupted";
}