#include "BlockBehaviour.hpp"
#include "src/Engine.hpp"
#include "src/voxels/Block.hpp"
#include "src/graphics/Sky.hpp"

BlockBehaviour BlockBehaviour::tall_grass {
    .on_block_set = [](BlockBehaviourContext context) {
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
};

BlockBehaviour BlockBehaviour::dry_bush {
    .on_block_set = [](BlockBehaviourContext context) {
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
};

BlockBehaviour BlockBehaviour::sand {
    .on_block_set = [](BlockBehaviourContext context) {
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
};

BlockBehaviour BlockBehaviour::grass {
    .on_random_tick = [](BlockBehaviourContext context) {
        if (context.y >= CHUNK_H-1) return;
        voxel above = Engine::pVoxelStorage->get_voxel(context.x, context.y+1, context.z);
        if (above.id == 0) return;
        Block& aboveBlock = Block::getBlockByVoxelId(above.id);
        // if above block is solid, turn grass into dirt
        if (aboveBlock.getBlockModel() == BlockModel::SOLID) {
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {1, 0}, false);
        }
    }
};

BlockBehaviour BlockBehaviour::leaves {
    .on_random_tick = [](BlockBehaviourContext context) {
        bool shouldFall = !Engine::pVoxelStorage->find_in_radius(context.x, context.y, context.z, 4, [](voxel v) {
            return v.id == 4;
        });
        if (shouldFall) {
            Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {0, 0}, false);
        }
    }
};

BlockBehaviour BlockBehaviour::dirt {
    .on_random_tick = [](BlockBehaviourContext context) {
        // turn dirt into grass if there is empty above and there is grass nearby
        if (context.y > CHUNK_H-1) return;
        voxel above = Engine::pVoxelStorage->get_voxel(context.x, context.y+1, context.z);
        if (above.id != 0) return;

        light lightAbove = Engine::pVoxelStorage->get_light(context.x, context.y+1, context.z);
        if ((float)lightAbove.getS() * Sky::get_sky_brightness() < 10.0f) return;

        bool hasGrassNearby = Engine::pVoxelStorage->find_in_radius(context.x, context.y, context.z, 1, [](voxel v) {
            return v.id == 3;
        });
        if (!hasGrassNearby) return;

        Engine::pVoxelStorage->set_voxel_soft(context.x, context.y, context.z, {3, 0}, false);
    }
};