#include "Block.hpp"

std::vector<Block *> Block::blocks = {};

Block::Block(std::string name, BlockModel model, std::vector<std::tuple<size_t, size_t>> UVs) : name(name),
                                                                                  model(model), 
                                                                                  UVs(UVs)
{
    Block::add_block(this);
    if (model == BlockModel::AIR) {
        this->opened_faces = {1, 1, 1, 1, 1, 1};
    } else {
        this->opened_faces = {0, 0, 0, 0, 0, 0};
    }
};

std::tuple<size_t, size_t> Block::getUV(int face){
    return UVs[0];
}