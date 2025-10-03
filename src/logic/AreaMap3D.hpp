#pragma once

#include "Array3D.hpp"
#include <functional>
#include <vector>

template <class T>
class AreaMap3D
{
public:
    AreaMap3D(int radius)
    {
        size = radius * 2 + 1; // Исправлено: +1 вместо +2
        firstBuffer = new Array3D<T*>(size, size, size);
        secondBuffer = new Array3D<T*>(size, size, size);
        for (int i = 0; i < size*size*size; i++) {
            firstBuffer->get_data()[i] = nullptr;
            secondBuffer->get_data()[i] = nullptr;
        }
    }

    ~AreaMap3D() {
        // Сначала удаляем все объекты
        for (int i = 0; i < size*size*size; i++) {
            delete firstBuffer->get_data()[i];
            delete secondBuffer->get_data()[i];
        }
        delete firstBuffer;
        delete secondBuffer;
    }

    T* get(int x, int y, int z) {
        int mx = x - offsetX + size/2;  
        int my = y - offsetY + size/2;
        int mz = z - offsetZ + size/2;
        
        if (!in_bounds(mx, my, mz)) {
            return nullptr;
        }
        return firstBuffer->get(mx, my, mz);
    }

    // unsafe! 
    T** get_volume(){
        return this->firstBuffer->get_data();
    }

    void set_out_callback(std::function<T*(int, int, int)> callback) {
        this->outCallback = callback;
    }

    void fill() {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int wx = x - size/2 + offsetX;
                    int wy = y - size/2 + offsetY;
                    int wz = z - size/2 + offsetZ;
                    T* newObj = outCallback(wx, wy, wz);
                    secondBuffer->set(x, y, z, newObj);
                }
            }
        }
        std::swap(firstBuffer, secondBuffer);
    }

    void translate(int dx, int dy, int dz) {

        if (dx == 0 && dy == 0 && dz == 0) return;

        std::vector<T*> to_delete;

        // Clear second buffer
        for (int i = 0; i < size*size*size; i++) {
            secondBuffer->get_data()[i] = nullptr;
        }

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int sourceX = x + dx;
                    int sourceY = y + dy;
                    int sourceZ = z + dz;

                    if (in_bounds(sourceX, sourceY, sourceZ)) {
                        secondBuffer->set(x, y, z, firstBuffer->get(sourceX, sourceY, sourceZ));
                        firstBuffer->set(sourceX, sourceY, sourceZ, nullptr);
                    } else {
                        int wx = x - size/2 + offsetX + dx;
                        int wy = y - size/2 + offsetY + dy;
                        int wz = z - size/2 + offsetZ + dz;
                        secondBuffer->set(x, y, z, outCallback(wx, wy, wz));
                    }
                }
            }
        }

        offsetX += dx;
        offsetY += dy;
        offsetZ += dz;

        std::swap(firstBuffer, secondBuffer);

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    if (secondBuffer->get(x, y, z) != nullptr) {
                        delete secondBuffer->get(x, y, z);
                    }
                }
            }
        }

            
    }

    bool is_inside(int x, int y, int z) {
        int mx = x - offsetX + size/2;
        int my = y - offsetY + size/2;
        int mz = z - offsetZ + size/2;
        return in_bounds(mx, my, mz);
    }
    
    // timely 
    int size;

// private:
    Array3D<T*>* firstBuffer;
    Array3D<T*>* secondBuffer;
    int offsetX = 0, offsetY = 0, offsetZ = 0;
    
    std::function<T*(int, int, int)> outCallback;

    bool in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
    }
};