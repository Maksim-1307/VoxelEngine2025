#include <iostream>
#include <chrono>

#include "Engine.hpp"

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
AreaMap2D<Chunk>* Engine::pChunkMap = nullptr;
Generator *Engine::pGenerator = nullptr;
VoxelStorage* Engine::pVoxelStorage = nullptr;
ChunksController* Engine::pChunksController = nullptr;
Terrain* Engine::pTerrain = nullptr;
Stats* Engine::pStats = nullptr;
Lighting* Engine::pLighting = nullptr;
WorldLoadingIndicator* Engine::pWorldLoadingIndicator = nullptr;
BlockMeshBuilder* Engine::pBlockMeshBuilder = nullptr;
Player* Engine::pPlayer = nullptr;
FPSCounter* Engine::pFPSCounter = nullptr;

void Engine::init()
{

    // Graphics
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

    // remove
    LoadingScreen* loadingScreen = new LoadingScreen();
    loadingScreen->on_enter();
    loadingScreen->draw();
    //


    // Blocks
    new Block("air", BlockModel::AIR,     {});
    new Block("dirt", BlockModel::SOLID,  {{0, 1}});
    new Block("stone", BlockModel::SOLID, {{1, 0}});
    new Block("grass", BlockModel::SOLID, {{1, 1}, {0, 0}, {0, 1}});
    new Block("oak_log", BlockModel::SOLID, {{2, 1}, {0, 2}, {2, 1}});
    new Block("leaves", BlockModel::FOLIAGE, {{1, 2}});
    new Block("red_lamp", BlockModel::SOLID, {{3, 2}}, true, {12, 0, 0});
    new Block("green_lamp", BlockModel::SOLID, {{3, 1}}, true, {0, 12, 0});
    new Block("blue_lamp", BlockModel::SOLID, {{3, 0}}, true, {0, 0, 12});
    new Block("lamp", BlockModel::SOLID, {{4, 0}}, true, {12, 12, 10});
    new Block("tall_grass", BlockModel::GRASS, {{0, 3}});
    new Block("dry_bush", BlockModel::GRASS, {{1, 3}});
    new Block("sand", BlockModel::SOLID, {{2, 3}});
    new Block("water", BlockModel::FOLIAGE, {{3, 3}});

    if (Settings::BLOCKS_BEHAVIOR) {
        BlockUpdater::get_instance().register_block_behaviour(10, BlockBehaviour::tall_grass);
        BlockUpdater::get_instance().register_block_behaviour(12, BlockBehaviour::sand);
        BlockUpdater::get_instance().register_block_behaviour(11, BlockBehaviour::dry_bush);
        BlockUpdater::get_instance().register_block_behaviour(3, BlockBehaviour::grass);
        if (Settings::FALLING_LEAVES) BlockUpdater::get_instance().register_block_behaviour(1, BlockBehaviour::dirt);
        if (Settings::FALLING_LEAVES) BlockUpdater::get_instance().register_block_behaviour(5, BlockBehaviour::leaves);
    }
    
    
    // Texts 
    std::string ss = "VoxelGame pre Alpha";
    Engine::pText = new Text(ss);
    std::string sss = "fps: ";
    Engine::pFpsText = new Text(sss);
    
    // World
    Engine::pGenerator = new Generator();
    Engine::pChunkMap = new AreaMap2D<Chunk>(Settings::LOAD_DISTANCE + 1);

    // Lighting
    Engine::pLighting = new Lighting(*Engine::pChunkMap);
    
    // Set up
    std::function<Chunk *(int, int)> gen_func = [](int x, int z) -> Chunk* 
    { 
        return Engine::pGenerator->generate_at(x, z);
    };
    Engine::pChunkMap->set_out_callback(gen_func);
    
    // Other
    Engine::pVoxelStorage = new VoxelStorage(Engine::pChunkMap);
    Engine::pChunksController = new ChunksController(Engine::pChunkMap, Engine::pCamera);
    Engine::pTerrain = new Terrain(*Engine::pVoxelStorage);
    Engine::pStats = new Stats();
    Engine::pWorldLoadingIndicator = new WorldLoadingIndicator(Engine::pChunkMap->get_chunks());
    Engine::pBlockMeshBuilder = new BlockMeshBuilder();
    Engine::pPlayer = new Player(glm::vec3(0, 70, 0), Engine::pCamera);
    Engine::pFPSCounter = new FPSCounter();

    // Input Callbacks
    Engine::pInputController->onPress(GLFW_KEY_TAB, []() {
        State::PLACING_VOXEL.id += 1;
        int count = Block::getBlocksCount();
        if (State::PLACING_VOXEL.id >= count) {
            State::PLACING_VOXEL.id = State::PLACING_VOXEL.id % count;
        }
        if (State::PLACING_VOXEL.id == 0) {
            State::PLACING_VOXEL.id = 1;
        }
    });
    Engine::pInputController->onPress(GLFW_KEY_ESCAPE, []() {
        State::MOUSE_CONTROL = !State::MOUSE_CONTROL;
    });
    Engine::pInputController->onPress(GLFW_KEY_SPACE, []() { 
        Engine::pPlayer->jump();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_SPACE, []() { 
        Engine::pPlayer->move_up();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_LEFT_SHIFT, []() {
        Engine::pPlayer->move_down();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_W, []() { 
        Engine::pPlayer->move_forward();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_A, []() { 
        Engine::pPlayer->move_left();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_S, []() { 
        Engine::pPlayer->move_backward();
    });
    Engine::pInputController->whileHolding(GLFW_KEY_D, []() { 
        Engine::pPlayer->move_right();
    });

    // remove
    loadingScreen->on_exit();
    //
}

void Engine::game_loop()
{
    using Clock = std::chrono::steady_clock;
    using Seconds = std::chrono::duration<double>;

    constexpr double TICK_RATE = 1.0 / 20.0;
    double accumulator = 0.0;

    std::cout << "game loop started\n";
    auto lastTime = Clock::now();

    while (!Engine::pWindow->should_close())
    {
        auto currentTime = Clock::now();
        double frameTime = Seconds(currentTime - lastTime).count();
        lastTime = currentTime;

        if (frameTime > 0.1) frameTime = 0.1;

        Engine::pInputController->update(static_cast<float>(frameTime));
        Engine::pPlayer->update();

        accumulator += frameTime;
        while (accumulator >= TICK_RATE) {
            Engine::pPlayer->prevPosition = Engine::pPlayer->hitbox->position;
            Engine::tick(TICK_RATE);
            accumulator -= TICK_RATE;
        }

        double alpha = accumulator / TICK_RATE;
        Engine::frame(frameTime, alpha);
    }
    Profiler::print_results();
    std::cout << "game loop interrupted\n";
}

void Engine::tick(double deltaTime)
{
    Time::tick();
    Sky::update();
    Physics::step(static_cast<float>(deltaTime));
    Engine::pChunksController->update();
    Engine::pWorldLoadingIndicator->update(Engine::pChunkMap->get_chunks());
    ChunksUpdater::get_instance().update();
}

void Engine::frame(double deltaTime, double alpha)
{
    Engine::pFPSCounter->update(static_cast<float>(deltaTime));
    Engine::pWindow->set_mouse_lock(State::MOUSE_CONTROL);

    glm::vec3 renderPos = glm::mix(
        Engine::pPlayer->prevPosition,
        Engine::pPlayer->hitbox->position,
        static_cast<float>(alpha)
    );
    Engine::pCamera->position = renderPos + glm::vec3(0.0f, 1.6f - 0.9f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // drawing terrain
    Engine::pMeshShader->use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = pCamera->getView();
    glm::mat4 projection = pCamera->getProjection();

    Engine::pMeshShader->set_matrix4("model", model);
    Engine::pMeshShader->set_matrix4("view", view);
    Engine::pMeshShader->set_matrix4("projection", projection);
    Engine::pMeshShader->set_texture("theTexture", Engine::pTexture->getID());
    Engine::pMeshShader->set_float("skyBrightness", 0.2f + 0.8f * Sky::get_sky_brightness());
    Engine::pMeshShader->set_vector3("fogColor", Sky::get_color());
    Engine::pMeshShader->set_float("fogStart", Settings::LOAD_DISTANCE * CHUNK_W * 0.6f);
    Engine::pMeshShader->set_float("fogDistance", Settings::LOAD_DISTANCE * CHUNK_W);
    Engine::pMeshShader->set_float("fogDensity", 3.0f);
    Engine::pMeshShader->set_vector3("sunVector", Sky::get_sun_vector());

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

    // updating stats
    glm::vec3 camPos = Engine::pCamera->position;
    bool obstacle = Engine::pTerrain->is_obstacle_at(camPos.x, camPos.y, camPos.z);
    Engine::pStats->set("Obstacle", obstacle ? "true" : "false");

    int skyLight = Engine::pVoxelStorage->get_light(floor(camPos.x), floor(camPos.y), floor(camPos.z)).getS();
    Engine::pStats->set("Sky Light", std::to_string(skyLight));

    Engine::pStats->set("X", std::to_string(camPos.x));
    Engine::pStats->set("Y", std::to_string(camPos.y));
    Engine::pStats->set("Z", std::to_string(camPos.z));
    Engine::pStats->set("Chunks count", std::to_string(Chunk::chunks));

    transform = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 250.0f, 0.0f));
    Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f)));
    Engine::pStats->draw();

    // drawing world loading indicator
    Engine::pSpriteShader->use();

    int windowWidth = Engine::pWindow->get_width();
    int windowHeight = Engine::pWindow->get_height();
    float rightPosition = windowWidth - 82.0f;
    float topPosition = 80.0f;

    transform = glm::translate(glm::mat4(1.0f), glm::vec3(rightPosition, topPosition, 0.0f));
    transform = glm::scale(transform, glm::vec3(64.0f, 64.0f, 1.0f));

    Engine::pSpriteShader->set_matrix4("projection", projection * transform);
    Engine::pSpriteShader->set_texture("theTexture", Engine::pWorldLoadingIndicator->texture->getID());
    Engine::pWorldLoadingIndicator->draw();

    // drawing placing block indicator
    topPosition = windowHeight - 90.0f;
    transform = glm::translate(glm::mat4(1.0f), glm::vec3(rightPosition, topPosition, 0.0f));
    transform = glm::scale(transform, glm::vec3(128.0f, 128.0f, 1.0f));

    Engine::pSpriteShader->set_matrix4("projection", projection * transform);
    Block& placingBlock = Block::getBlockByVoxelId(State::PLACING_VOXEL.id);
    Engine::pSpriteShader->set_texture("theTexture", placingBlock.getIcon().getTexture()->getID());
    placingBlock.getIcon().draw();

    glfwSwapBuffers(Engine::pWindow->get_glfw_window());
    glfwPollEvents();
}
