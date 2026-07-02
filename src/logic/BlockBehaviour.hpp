#pragma once

#include <iostream>
#include <functional>

struct BlockBehaviourContext {
    int x, y, z;
};

class BlockBehaviour {
public:
    // BlockBehaviour() = default;
    // BlockBehaviour(std::function<void(BlockBehaviourContext)> on_block_set) : on_block_set(on_block_set) {};

    std::function<void(BlockBehaviourContext)> on_block_set;
    std::function<void(BlockBehaviourContext)> on_random_tick;

    static BlockBehaviour grass;
    static BlockBehaviour leaves;
    static BlockBehaviour sand;
    static BlockBehaviour dry_bush;
    static BlockBehaviour tall_grass;
};