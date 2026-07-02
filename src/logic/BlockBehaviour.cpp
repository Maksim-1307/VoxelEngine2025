#include "BlockBehaviour.hpp"
#include "src/Engine.hpp"
#include "src/voxels/Block.hpp"

BlockBehaviour BlockBehaviour::grass (
    [](BlockBehaviourContext context) {
        if (context.y == 0) {
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0});
            return;
        }
        if (context.y >= CHUNK_H) {
            return;
        }
        voxel below = Engine::pVoxelStorage->get_voxel(context.x, context.y-1, context.z);
        if (below.id != 3 && below.id != 1) { // dirt or grass
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0});
        }
    }
);

BlockBehaviour BlockBehaviour::dry_bush (
    [](BlockBehaviourContext context) {
        if (context.y == 0) {
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0});
            return;
        }
        if (context.y >= CHUNK_H) {
            return;
        }
        voxel below = Engine::pVoxelStorage->get_voxel(context.x, context.y-1, context.z);
        Block &belowBlock = Block::getBlockByVoxelId(below.id);
        if (belowBlock.getBlockModel() != BlockModel::SOLID) { // can be placed only on solid blocks
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0});
        }
    }
);

BlockBehaviour BlockBehaviour::sand (
    [](BlockBehaviourContext context) {
        if (context.y <= 0) {
            return;
        }
        int y = context.y;
        while (y > 0) {
            voxel below = Engine::pVoxelStorage->get_voxel(context.x, y-1, context.z);
            Block& belowBlock = Block::getBlockByVoxelId(below.id);
            if (belowBlock.getBlockModel() == BlockModel::SOLID) {
                break;
            }
            y--;
        }
        if (y == context.y) {
            return;
        }
        Engine::pVoxelStorage->set_voxel_soft(context.x, y, context.z, {12, 0}, false);
        Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0}, true);
    }
);

BlockBehaviour BlockBehaviour::leaves;