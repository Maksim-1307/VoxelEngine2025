#pragma once

#include <iostream>
#include <stdint.h>
#include <string.h>
#include <random>
#include <functional>
#include "src/voxels/Chunk.hpp"

enum class GENERATION_TYPE {
    PERLIN_NOISE_3D,
    DEFAULT
};

class Generator {
    public:
    Generator(std::string seed="")
    {
        if (seed == ""){
            std::srand(std::time(nullptr)); 
            this->seed = (std::rand() << 16) | std::rand();
        } else {
            this->seed = crc32(seed.c_str());
        }
    };

    Chunk* generate_at(int x, int z);
    void generate_ambient(int x, int z);
    void generate_tree(int x, int y, int z);

    Chunk* perlin_noise_2d(int x, int y, int z);
    Chunk* perlin_noise_3d(int x, int y, int z);
    Chunk* terrain_with_caves(int x, int y, int z);
    
    private:
        uint32_t crc32(const char *str)
        {
            uint32_t crc = 0xFFFFFFFF;
            for (size_t i = 0; str[i] != '\0'; i++)
            {
                crc ^= str[i];
                for (int j = 0; j < 8; j++)
                {
                    crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
                }
            }
            return ~crc;
        };
        float random(int x, int y, unsigned int seed) {
            uint32_t h = static_cast<uint32_t>(seed);
            h ^= static_cast<uint32_t>(x) * 0x9e3779b9;
            h ^= static_cast<uint32_t>(y) * 0x85ebca6b;
            
            // Finalize hash - avalanche bits
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            h ^= h >> 16;
            
            return static_cast<float>(h) / 4294967295.0f; 
        }
        uint32_t seed;
};