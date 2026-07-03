#include "Block.hpp"
#include "src/Engine.hpp"

std::vector<Block *> Block::blocks = {};

Block::Block(
    std::string name, 
    BlockModel model, 
    std::vector<std::tuple<size_t, size_t>> UVs,
    bool emissive, 
    std::array<uint8_t, 3> emission
) : name(name), model(model), emissive(emissive), emission(emission)                             
{
    set_UVs(UVs);
    Block::add_block(this);
    switch (model) {
        case BlockModel::AIR: case BlockModel::FOLIAGE: case BlockModel::GRASS:
            this->opened_faces = {1, 1, 1, 1, 1, 1};
            this->lightPassing = true;
            break;
        case BlockModel::SOLID:
            this->opened_faces = {0, 0, 0, 0, 0, 0};
            break;
        default:
            this->opened_faces = {1, 1, 1, 1, 1, 1};
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

const std::vector<AABB> Block::getAABBs(int x, int y, int z, IteractionType type){
    voxel vox = Engine::pVoxelStorage->get_voxel(x, y, z);
    BlockModel model = Block::getBlockByVoxelId(vox.id).getBlockModel();
    switch (model) {
        case BlockModel::AIR:
            return {};
        case BlockModel::SOLID:
            return {AABB(glm::vec3(1.0f), PhysicsMaterial::SOLID)};
        case BlockModel::FOLIAGE:
            if (type == RAYCAST) { // remove decoupling in future
                return {AABB(glm::vec3(1.0f))};
            } else {
                return {AABB(glm::vec3(1.0f), PhysicsMaterial::LEAVES)};
            }
        case BlockModel::GRASS:
            if (type == RAYCAST) {
                // delta = 0.5 - 0.5 * cos(45 degrees)
                const float d = 0.146f;
                return {AABB(glm::vec3(d, 0, d), glm::vec3(1.0f-d))};
            } else {
                return {};
            }
        default:
            return {};
    }
}