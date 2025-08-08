#include "Block.hpp"

std::vector<Block *> Block::blocks = {};

Block::Block(std::string name, BlockModel model, std::vector<std::tuple<size_t, size_t>> UVs) 
: name(name),
model(model)                                                                                
{
    set_UVs(UVs);
    Block::add_block(this);
    if (model == BlockModel::AIR) {
        this->opened_faces = {1, 1, 1, 1, 1, 1};
    } else {
        this->opened_faces = {0, 0, 0, 0, 0, 0};
    }
};

std::tuple<size_t, size_t> Block::getUV(int face){
    return UVs[face];
}

void Block::set_UVs(std::vector<std::tuple<size_t, size_t>> &UV){
    switch (UV.size()){
        case 0:
            UVs = {};
            break;
        case 1:
            UVs = {
                UV[0], // X+
                UV[0], // X-
                UV[0], // Y+ (top)
                UV[0], // Y- (bottom)
                UV[0], // Z+
                UV[0], // Z-
            };
            break;
        case 2:
            UVs = {
                UV[1], // X+
                UV[1], // X-
                UV[0], // Y+ (top)
                UV[0], // Y- (bottom)
                UV[1], // Z+
                UV[1], // Z-
            };
            break;
        case 3:
            UVs = {
                UV[1], // X+
                UV[1], // X-
                UV[0], // Y+ (top)
                UV[2], // Y- (bottom)
                UV[1], // Z+
                UV[1], // Z-
            };
            break;
        case 6:
            UVs = {
                UV[0], // X+
                UV[1], // X-
                UV[2], // Y+ (top)
                UV[3], // Y- (bottom)
                UV[4], // Z+
                UV[5], // Z-
            };
            break;
        default:
            std::string errMsg = "ERROR: " + std::to_string(UV.size()) + " different faces is not supported yet";
            throw std::invalid_argument(errMsg);
            break;
    }
}
