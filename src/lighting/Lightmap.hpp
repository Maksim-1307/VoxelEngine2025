#pragma once 

#include "light.hpp"
#include "src/logic/Array3D.hpp"

class Lightmap {
public:
    Lightmap(int X, int Y, int Z) : map(X, Y, Z){};
    ~Lightmap(){};
    Array3D<light> map;

    void clear(){
        map.clear({0});
    }

    void set(int x, int y, int z, light val){
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
        map.set(x, y, z, l);
    }
    void setG(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setG(val);
        map.set(x, y, z, l);
    }
    void setB(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setB(val);
        map.set(x, y, z, l);
    }
    void setS(int x, int y, int z, int val){
        light l = map.get(x, y, z);
        l.setS(val);
        map.set(x, y, z, l);
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
};