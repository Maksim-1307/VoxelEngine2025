#pragma once

struct ChunkPos {
    int x, z;
    int first () {
        return x;
    }
        
    int second () {
        return z;
    }
        
};