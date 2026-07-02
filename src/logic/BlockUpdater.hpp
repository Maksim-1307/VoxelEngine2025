#pragma once
#include <iostream>
#include <array>
#include "src/voxels/Block.hpp"
#include "src/logic/BlockBehaviour.hpp"

class BlockUpdater {
private:
    BlockUpdater();
    ~BlockUpdater(){}

    std::vector<BlockBehaviour> functionRegistry;
public:
    static BlockUpdater& get_instance() {
        static BlockUpdater instance;
        return instance;
    }

    BlockBehaviour& register_block_behaviour(int blockId, BlockBehaviour behaviour) {
        functionRegistry[blockId] = behaviour;
        return functionRegistry[blockId];
    }

    void on_block_set(int x, int y, int z);
};