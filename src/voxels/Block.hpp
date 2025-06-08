#pragma once

#include <iostream>
#include "BlockModel.hpp"
#include "voxel.hpp"


class Block{
public:
    Block(std::string name, BlockModel model); //: name(name), model(model);
    const std::string name;

    BlockModel getBlockModel() const {
        return model;
    }

    static Block getBlockByVoxelId(size_t id)
    {
        return blocks[id];
    }
    std::array<bool, 6> opened_faces(int x, int y, int z){
        return {0, 0, 0, 0, 0, 0};
    }

    private : 
    const BlockModel model;
    uint8_t VoxelId : ID_SIZE;
    // stores blocks data in format blocks[voxelId] -> Block
    static std::vector<Block> blocks;
};


/*
using VoxelGame.Logic;
using OpenTK.Mathematics;

namespace VoxelGame.Voxels
{
public
    class Block
    {
    public
        static Dictionary<byte, Block> blocks = new Dictionary<byte, Block>();
    public
        byte voxelId;
    public
        BlockModel blockModel;
        public(uint x, uint y)[] UVs;
    public
        bool[] OpenedFaces = [ false, false, false, false, false, false ];

    public
        Block(byte voxelId, BlockModel blockModel, (uint x, uint y)[] UVs)
        {
            this.voxelId = voxelId;
            this.blockModel = blockModel;
            this.UVs = UVs;
            this.OpenedFaces = getOpenedFaces(blockModel);
            if (UVs.Count() == 0)
                throw new ArgumentException("Block must have at least one texture UV");

            blocks.Add(voxelId, this);
        }

    private
        static bool[] getOpenedFaces(BlockModel blockModel)
        {
            if (blockModel == BlockModel.Air)
                return [ true, true, true, true, true, true ];
            if (blockModel == BlockModel.Foliage)
                return [ true, true, true, true, true, true ];
            return [ false, false, false, false, false, false ];
        }

    public
        static Block GetBlockByVoxelId(byte id)
        {
            if (!blocks.ContainsKey(id))
                return blocks[0];
            return blocks[id];
        }

        public(uint x, uint y) GetUV(byte face)
        {
            if (face < 0 || face > 5)
                throw new ArgumentException("Face is a number between 0 and 6");
            switch (UVs.Count())
            {
            case 1:
                // like a dirt block
                return UVs[0];
            case 2:
                // like a log block. Top and bottom are texture 0, sides are texture 1
                if (face == 2 || face == 3)
                    return UVs[0];
                return UVs[1];
            case 3:
                // like a grass block
                if (face == 2)
                    return UVs[0]; // top face
                if (face == 3)
                    return UVs[1]; // bottom face
                return UVs[2];     // side face
            case 6:
                // every side is unque
                return UVs[(face + 2) % 6];
            }
            return UVs[0];
        }

    public
        static AABB[] GetAABBs(Voxel vox)
        {
            Block block = GetBlockByVoxelId(vox.Id);
            if (block.blockModel == BlockModel.Air)
                return [];
            if (block.blockModel == BlockModel.Foliage)
                return [];
            return [new AABB(new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f))];
        }
    }

    public enum BlockModel {
        Air,
        Cube,
        XSprite,
        Stairs,
        HalfBlock,
        Fluid,
        Foliage
    }
}*/