#pragma once

#include <iostream>
#include "src/voxels/Chunk.hpp"

class Generator {
    public:
    Generator(std::string seed)
        : seed(seed)
    {};

    Chunk* generate_at(int x, int y, int z);

    private:
    std::string seed;
};