#include "BlockBehaviour.hpp"
#include "src/Engine.hpp"

BlockBehaviour BlockBehaviour::grass (
    [](BlockBehaviourContext context) {
        if (context.y == 0) {
            Engine::pVoxelStorage->set_voxel(context.x, context.y, context.z, {0, 0});
            return;
        }
        if (context.y >= CHUNK_H) {
            return;
        }
        voxel below = Engine::pVoxelStorage->get_voxel(context.x, context.y-1, context.z);
        if (below.id != 3 && below.id != 1) { 
            Engine::pVoxelStorage->set_voxel(context.x, context.y, context.z, {0, 0});
        }
    }
);

BlockBehaviour BlockBehaviour::leaves;
BlockBehaviour BlockBehaviour::sand;