#include "BlockMeshBuilder.hpp"

#include "src/voxels/Block.hpp"
#include "src/graphics/Mesh.hpp"
#include "src/Engine.hpp"

BlockMeshBuilder::BlockMeshBuilder() {}

uptr<Mesh> BlockMeshBuilder::buildMesh(Block& block) {
    vertices = {};
    indices = {};

    indexOffset = 0;
    
    switch (block.getBlockModel()) {
        case BlockModel::SOLID: case BlockModel::FOLIAGE:
            CubeModel(block);
            break; 
        case BlockModel::GRASS:
            GrassModel(block);
        default:
            break;
    }
    return make_uptr<Mesh>(std::move(vertices), std::move(indices));
}

void BlockMeshBuilder::CubeModel(Block& block) {

    for (int face = 0; face < 6; face++)
    {
        size_t UVx = std::get<0>(block.getUV(face));
        size_t UVy = std::get<1>(block.getUV(face));
        glm::vec2 uv = glm::vec2((float)UVx / ATLAS_SIZE, (float)UVy / ATLAS_SIZE);
        // _face = face;
        switch (face)
        {
        // X+
        case 0:
            vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            vertex(1.0f, 0.0f, 1.0f, uv.x, uv.y);
            vertex(1.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            break;
        // X-
        case 1:
            vertex(0.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y);
            vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(0.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            break;
        // Y+
        case 2:
            vertex(0.0f, 1.0f, 0.0f, uv.x, uv.y);
            vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 1.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            break;
        // Y-
        case 3:
            vertex(0.0f, 0.0f, 0.0f, uv.x, uv.y);
            vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 0.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            break;
        // Z+
        case 4:
            vertex(0.0f, 0.0f, 1.0f, uv.x, uv.y);
            vertex(0.0f, 1.0f, 1.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 1.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 0.0f, 1.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            break;
        // Z-
        case 5:
            vertex(0.0f, 0.0f, 0.0f, uv.x, uv.y);
            vertex(0.0f, 1.0f, 0.0f, uv.x, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 1.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
            vertex(1.0f, 0.0f, 0.0f, uv.x + 1.0f / ATLAS_SIZE, uv.y);
            break;
        }

        // direct and reverse order (when polygon must be rendered from other side)
        if (face % 2 == 0)
        {
            index(0, 1, 3, 1, 2, 3);
        }
        else
        {
            index(3, 1, 0, 3, 2, 1);
        }
    }
}

void BlockMeshBuilder::GrassModel(Block& block)
{
    size_t UVx = std::get<0>(block.getUV(0));
    size_t UVy = std::get<1>(block.getUV(0));
    glm::vec2 uv = glm::vec2((float)UVx / ATLAS_SIZE, (float)UVy / ATLAS_SIZE);

    // delta = 0.5 - 0.5 * cos(45 degrees)
    const float d = 0.146f;

    // _face = 6; // Grass model doesnt have faces. Use inner light calculation for all vertices

    // First sprite
    vertex(d, 0.0f, d, uv.x, uv.y);
    vertex(d, 1.0f, d, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d, 1.0f, 1.0f-d, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d, 0.0f, 1.0f-d, uv.x + 1.0f / ATLAS_SIZE, uv.y);

    index(0, 1, 3, 1, 2, 3);

    // Second sprite
    vertex(d, 0.0f, 1.0f-d, uv.x, uv.y);
    vertex(d, 1.0f, 1.0f-d, uv.x, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d, 1.0f, d, uv.x + 1.0f / ATLAS_SIZE, uv.y + 1.0f / ATLAS_SIZE);
    vertex(1.0f-d, 0.0f, d, uv.x + 1.0f / ATLAS_SIZE, uv.y);

    index(0, 1, 3, 1, 2, 3);
}

void BlockMeshBuilder::vertex(float x, float y, float z, float u, float v)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    vertices.push_back(u);
    vertices.push_back(v);
    vertices.push_back(pack_to_float(15, _face));
}

void BlockMeshBuilder::index(uint a, uint b, uint c, uint d, uint e, uint f)
{
    indices.push_back(indexOffset + a);
    indices.push_back(indexOffset + b);
    indices.push_back(indexOffset + c);
    indices.push_back(indexOffset + d);
    indices.push_back(indexOffset + e);
    indices.push_back(indexOffset + f);
    indexOffset += 4;
}