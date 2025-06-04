#pragma once

#include "graphics/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshRenderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"
#include "graphics/CameraController.hpp"
#include "graphics/Canvas.hpp"
#include "graphics/Texture.hpp"
#include "logic/FPSCounter.hpp"

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
        static CameraController *pCameraController;
        static Canvas* pCanvas;
        static Texture* pTexture;
};