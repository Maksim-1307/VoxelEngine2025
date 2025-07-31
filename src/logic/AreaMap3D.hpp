#pragma once

#include "Array3D.hpp"

template <class T>
class AreaMap3D
{
public:
    AreaMap3D(int radius)
    {
        size = radius * 2 + 1; 
        firstBuffer = new Array3D<T*>(size, size, size);
        secondBuffer = new Array3D<T*>(size, size, size);
        for (int i = 0; i < size*size*size; i++) {
            firstBuffer->get_data()[i] = nullptr;
            secondBuffer->get_data()[i] = nullptr;
        }
    }

    ~AreaMap3D() {
        delete firstBuffer;
        delete secondBuffer;
    }

    T* get(int x, int y, int z) {
        int mx = x + size/2 - offsetX;
        int my = y + size/2 - offsetY;
        int mz = z + size/2 - offsetZ;
        
        if (!in_bounds(mx, my, mz)) {
            return nullptr;
        }
        return firstBuffer->get(mx, my, mz);
    }

    void set_out_callback(std::function<T*(int, int, int)> callback) {
        this->outCallback = callback;
    }

    T** get_data() const {
        return this->firstBuffer->get_data();
    }

    void fill() {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int wx = x - size/2 + offsetX;
                    int wy = y - size/2 + offsetY;
                    int wz = z - size/2 + offsetZ;
                    secondBuffer->set(x, y, z, outCallback(wx, wy, wz));
                }
            }
        }
        std::swap(firstBuffer, secondBuffer);
    }

    void translate(int dX, int dY, int dZ) {
        if (dX == 0 && dY == 0 && dZ == 0) return;

        // no idea why, but when you multiply by -1 it works great
        dX *= -1;
        dY *= -1;
        dZ *= -1;
        
        for (int i = 0; i < size*size*size; i++) {
            secondBuffer->get_data()[i] = nullptr;
        }

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int oldX = x - dX;
                    int oldY = y - dY;
                    int oldZ = z - dZ;
                    
                    if (in_bounds(oldX, oldY, oldZ)) {
                        secondBuffer->set(x, y, z, firstBuffer->get(oldX, oldY, oldZ));
                    }
                }
            }
        }

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    if (secondBuffer->get(x, y, z) == nullptr) {
                        int wx = x - size/2 + offsetX + dX;
                        int wy = y - size/2 + offsetY + dY;
                        int wz = z - size/2 + offsetZ + dZ;
                        secondBuffer->set(x, y, z, outCallback(wx, wy, wz));
                    }
                }
            }
        }
        // and here too 
        offsetX -= dX;
        offsetY -= dY;
        offsetZ -= dZ;

        std::swap(firstBuffer, secondBuffer);
    }

    int get_size() const {
        return this->size;
    }

    bool is_inside(int x, int y, int z) {
        int mx = x - offsetX + size/2;
        int my = y - offsetY + size/2;
        int mz = z - offsetZ + size/2;
        return mx >= 0 && my >= 0 && mz >= 0 && 
               mx < size && my < size && mz < size;
    }

private:
    Array3D<T*>* firstBuffer;
    Array3D<T*>* secondBuffer;
    int offsetX = 0, offsetY = 0, offsetZ = 0;
    int size;
    std::function<T*(int, int, int)> outCallback;

    bool in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
    }
};