#pragma once

#include <iostream>
#include <stdint.h>
#include <string.h>
#include <random>
#include "src/voxels/Chunk.hpp"

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

    Chunk* generate_at(int x, int y, int z);

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
        uint32_t seed;
};