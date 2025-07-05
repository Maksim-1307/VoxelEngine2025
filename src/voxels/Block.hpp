#pragma once

#include <iostream>
#include "BlockModel.hpp"
#include "voxel.hpp"


class Block{
public:
    Block(std::string name, BlockModel model, std::vector<std::tuple<size_t, size_t>> UVs); //: name(name), model(model);
    const std::string name;

    BlockModel getBlockModel() const {
        return model;
    }

    static const Block& getBlockByVoxelId(size_t id) 
    {
        return *blocks[id];
    }
    std::array<bool, 6> opened_faces;

    std::tuple<size_t, size_t> getUV(int face);

    private : 
    static void add_block(Block *block)
    {
        Block::blocks.push_back(block);
        block->voxelId = Block::blocks.size() - 1;
    }
    void set_UVs(std::vector<std::tuple<size_t, size_t>>& UV);
    std::array<std::tuple<size_t, size_t>, 6> UVs;
    const BlockModel model;
    uint8_t voxelId : ID_SIZE;
    // stores blocks data in format blocks[voxelId] -> Block
    static std::vector<Block*> blocks;
};

