#pragma once

#include <iostream>
#include "BlockModel.hpp"
#include "voxel.hpp"
#include "src/physics/AABB.hpp"


class Block{
public :
    Block(std::string name, BlockModel model, std::vector<std::tuple<size_t, size_t>> UVs); //: name(name), model(model);
    
    const std::string name;
    std::array<bool, 6> opened_faces;

    BlockModel getBlockModel() const { return model; }
    std::tuple<size_t, size_t> getUV(int face);

    static const Block& getBlockByVoxelId(size_t id) 
    {
        return *blocks[id];
    }
    static const std::vector<AABB> getAABBs(voxel vox){
        BlockModel model = Block::getBlockByVoxelId(vox.id).getBlockModel();
        switch (model) {
            case BlockModel::AIR:
                return {};
            case BlockModel::SOLID:
                return {AABB(glm::vec3(1.0f))};
            default:
                return {};
        }
    }

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

