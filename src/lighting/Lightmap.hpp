#pragma once 

#include "../../include/rapidhash.h"
#include "light.hpp"
#include "src/logic/Array3D.hpp"
#include "src/logic/Array2D.hpp"

class Lightmap {
    Array3D<light> map;

    // size
    const int X, Y, Z;

    // Edge cache. Used to compare them (using hash) to determine whether or not
    // neighboring chunks need to be updated.
    Array2D<light> pos_x_cache;
    Array2D<light> neg_x_cache;
    Array2D<light> pos_z_cache;
    Array2D<light> neg_z_cache;

    void setEdgeCache(int x, int y, int z, light val) {
        if (x == 0) neg_x_cache.set(z, y, val);
        if (x == X) pos_x_cache.set(z, y, val);
        if (z == 0) neg_z_cache.set(x, y, val);
        if (z == Z) pos_z_cache.set(x, y, val);
    }

    uint64_t getHashOf(Array2D<light>& edge) {
        return rapidhash(
            edge.get_data(),
            edge.getSizeX() * edge.getSizeY() * sizeof(light)
        );
    }

public:
    Lightmap(int X, int Y, int Z)
        : map(X, Y, Z), X(X), Y(Y), Z(Z),
          pos_x_cache(X, Y),
          neg_x_cache(X, Y),
          pos_z_cache(Z, Y),
          neg_z_cache(Z, Y) 
    {};

    ~Lightmap(){};
    

    void clear(){
        map.clear({0});
        pos_x_cache.clear({0});
        neg_x_cache.clear({0});
        pos_z_cache.clear({0});
        neg_z_cache.clear({0});
    }

    // Don't call map.set directly, use this wrapper because it updates edge cache
    inline void set(int x, int y, int z, light val){
        bool onEdge = (x == 0) | (x == X) |
              (y == 0) | (y == Y) |
              (z == 0) | (z == Z);
        if (onEdge) setEdgeCache(x, y, z, val);
        map.set(x, y, z, val);
    }
    void set(int x, int y, int z, int channel, uint8_t val) {
        switch (channel) {
            case 0:
                setR(x, y, z, val);
                break;
            case 1:
                setG(x, y, z, val);
                break;
            case 2:
                setB(x, y, z, val);
                break;
            case 3:
                setS(x, y, z, val);
                break;
        }
    }
    void setR(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setR(val);
        set(x, y, z, l);
    }
    void setG(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setG(val);
        set(x, y, z, l);
    }
    void setB(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setB(val);
        set(x, y, z, l);
    }
    void setS(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setS(val);
        set(x, y, z, l);
    }

    light get(int x, int y, int z) const {
        return map.get(x, y, z);
    }
    uint8_t get(int x, int y, int z, int channel) {
        return map.get(x, y, z).get(channel);
    }
    uint8_t getR(int x, int y, int z) const {
        return map.get(x, y, z).getR();
    }
    uint8_t getG(int x, int y, int z) const {
        return map.get(x, y, z).getG();
    }
    uint8_t getB(int x, int y, int z) const {
        return map.get(x, y, z).getB();
    }
    uint8_t getS(int x, int y, int z) const {
        return map.get(x, y, z).getS();
    }
    uint64_t getHashOfPositiveXEdge() {
        return getHashOf(pos_x_cache);
    }
    uint64_t getHashOfNegativeXEdge() {
        return getHashOf(neg_x_cache);
    }
    uint64_t getHashOfPositiveZEdge() {
        return getHashOf(pos_z_cache);
    }
    uint64_t getHashOfNegativeZEdge() {
        return getHashOf(neg_z_cache);
    }
};