#pragma once

#include "graphics/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshRenderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Canvas.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Text.hpp"
#include "graphics/ChunkMeshBuilder.hpp"
#include "logic/FPSCounter.hpp"
#include "logic/InputController.hpp"
#include "voxels/Chunk.hpp"
#include "logic/AreaMap3D.hpp"
#include "voxels/Generator.hpp"
#include "voxels/VoxelStorage.hpp"
#include "logic/ChunksController.hpp"


class Engine {
    public:
        Engine();
        void init();
        void game_loop();

        static Window* pWindow;
        static Shader* pSpriteShader;
        static Shader *pTextShader;
        static Shader* pMeshShader;
        static Mesh* pMesh;
        static Mesh* pSprite;
        static MeshRenderer* pRenderer;
        static MeshRenderer *pSpriteRenderer;
        static Camera* pCamera;
        static InputController *pInputController;
        static Canvas* pCanvas;
        static Texture* pTexture;
        static Text* pText;
        static Chunk* pChunk;
        static ChunkMeshBuilder *pChunkMeshBuilder;
        static AreaMap3D<Chunk>* pChunkMap;
        static Generator* pGenerator;
        static VoxelStorage* pVoxelStorage;
        static ChunksController* pChunksController;
};