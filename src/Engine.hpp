#pragma once

#include "graphics/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshRenderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"

class Engine {
    public:
        Engine();
        void init();
        void game_loop();

        static Window* pWindow;
        static Shader* pSpriteShader;
        static Shader* pMeshShader;
        static Mesh* pMesh;
        static MeshRenderer* pRenderer;
        static Camera* pCamera;
};