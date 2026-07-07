#pragma once

#include <functional>

struct ChunkPos {
    int x, z;
    int first () const {
        return x;
    }
        
    int second () const {
        return z;
    }

    bool operator==(const ChunkPos& other) const {
        return x == other.x && z == other.z;
    }
};

namespace std {
    template<>
    struct hash<ChunkPos> {
        size_t operator()(const ChunkPos& pos) const {
            return hash<int>()(pos.x) ^ (hash<int>()(pos.z) << 1);
        }
    };
}