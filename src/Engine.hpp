#pragma once
#include "logic/pointers.hpp"

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
#include "logic/AreaMap2D.hpp"
#include "voxels/Generator.hpp"
#include "voxels/VoxelStorage.hpp"
#include "logic/ChunksController.hpp"
#include "physics/Terrain.hpp"
#include "graphics/Stats.hpp"
#include "lighting/Lighting.hpp"
#include "graphics/WorldLoadingIndicator.hpp"
#include "logic/State.hpp"
#include "graphics/BlockMeshBuilder.hpp"
#include "entities/Player.hpp"
#include "physics/Physics.hpp"
#include "src/time/Profiler.hpp"

// class State;

#include "logic/Iterator.hpp"

class Engine {
    public:
        Engine();
        void init();
        void game_loop();

        static Window* pWindow;
        static Shader* pSpriteShader;
        static Shader *pTextShader;
        static Shader* pMeshShader;
        static Mesh* pSprite;
        static MeshRenderer* pRenderer;
        static MeshRenderer *pSpriteRenderer;
        static Camera* pCamera;
        static InputController *pInputController;
        static Canvas* pCanvas;
        static Texture* pTexture;
        static Text* pText;
        static Text* pFpsText;
        static Chunk* pChunk;
        static ChunkMeshBuilder *pChunkMeshBuilder;
        static AreaMap2D<Chunk>* pChunkMap;
        static Generator* pGenerator;
        static VoxelStorage* pVoxelStorage;
        static ChunksController* pChunksController;
        static Terrain* pTerrain;
        static Stats* pStats;
        static Lighting* pLighting;
        static WorldLoadingIndicator* pWorldLoadingIndicator;
        static BlockMeshBuilder* pBlockMeshBuilder;
        static Player* pPlayer;
        // static BlockIcon* pBlockIcon;
};