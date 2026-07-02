#include "src/logic/BlockUpdater.hpp"
#include "src/Engine.hpp"

BlockUpdater::BlockUpdater() {
    this->functionRegistry.resize(Block::getBlocksCount());
}

void BlockUpdater::on_block_set(int x, int y, int z) {
    int blockId = Engine::pVoxelStorage->get_voxel(x, y, z).id;
    if (blockId >= functionRegistry.size()) {
        return;
    }
    BlockBehaviour& behaviour = functionRegistry[blockId];
    if (behaviour.on_block_set) {
        BlockBehaviourContext context{x, y, z};
        behaviour.on_block_set(context);
    } else  {
        std::cout << "No behaviour registered for block ID " << blockId << std::endl;
    }
}