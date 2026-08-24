#pragma once

#include <mutex>
#include "Array2D.hpp"
#include "Iterator.hpp"
#include <functional>
#include <vector>

template <class T>
class AreaMap2D
{
public:
    AreaMap2D(int radius)
    {
        size = radius * 2 + 1; 
        firstBuffer = new Array2D<T*>(size, size);
        secondBuffer = new Array2D<T*>(size, size);
        for (int i = 0; i < size*size; i++) {
            firstBuffer->get_data()[i] = nullptr;
            secondBuffer->get_data()[i] = nullptr;
        }
    }

    ~AreaMap2D() {
        for (int i = 0; i < size*size; i++) {
            delete firstBuffer->get_data()[i];
            delete secondBuffer->get_data()[i];
        }
        delete firstBuffer;
        delete secondBuffer;
    }

    T* get(int x, int z) {
        std::lock_guard lock(mtx);
        int mx = x - offsetX + size/2;  
        int mz = z - offsetZ + size/2;
        
        if (!in_bounds(mx, mz)) {
            return nullptr;
        }
        return firstBuffer->get(mx, mz);
    }

    // unsafe!
    T** get_volume(){
        std::lock_guard lock(mtx);
        return this->firstBuffer->get_data();
    }

    void set_out_callback(std::function<T*(int, int)> callback) {
        this->outCallback = callback;
    }

    void fill() {
        std::lock_guard lock(mtx);
        for (int x = 0; x < size; x++) {
            for (int z = 0; z < size; z++) {
                int wx = x - size/2 + offsetX;
                int wz = z - size/2 + offsetZ;
                T* newObj = outCallback(wx, wz);
                secondBuffer->set(x, z, newObj);
            }
        }
        std::swap(firstBuffer, secondBuffer);
    }

    void translate(int dx, int dz) {
        std::lock_guard lock(mtx);

        if (dx == 0 && dz == 0) return;

        std::vector<T*> to_delete;

        for (int i = 0; i < size*size; i++) {
            secondBuffer->get_data()[i] = nullptr;
        }

        for (int x = 0; x < size; x++) {
            for (int z = 0; z < size; z++) {
                int sourceX = x + dx;
                int sourceZ = z + dz;

                if (in_bounds(sourceX, sourceZ)) {
                    secondBuffer->set(x, z, firstBuffer->get(sourceX, sourceZ));
                    firstBuffer->set(sourceX, sourceZ, nullptr);
                } else {
                    int wx = x - size/2 + offsetX + dx;
                    int wz = z - size/2 + offsetZ + dz;
                    secondBuffer->set(x, z, outCallback(wx, wz));
                }
            }
        }

        offsetX += dx;
        offsetZ += dz;

        std::swap(firstBuffer, secondBuffer);

        for (int x = 0; x < size; x++) {
            for (int z = 0; z < size; z++) {
                if (secondBuffer->get(x, z) != nullptr) {
                    delete secondBuffer->get(x, z);
                }
            }
        }   
    }

    bool is_inside(int x, int z) {
        std::lock_guard lock(mtx);
        int mx = x - offsetX + size/2;
        int mz = z - offsetZ + size/2;
        return in_bounds(mx, mz);
    }
    
    std::vector<T*> padding_chunks(int level){
        std::lock_guard lock(mtx);
        return Iterator<T*>::padding(this->firstBuffer, level);
    }
    std::vector<T*> chunks_in_radius(int radius) {
        std::lock_guard lock(mtx);
        return Iterator<T*>::in_radius(this->firstBuffer, radius);
    }

    Array2D<T*>* get_chunks() {
        std::lock_guard lock(mtx);
        return this->firstBuffer;
    }

    int size;
    std::mutex mtx;

// private:
    Array2D<T*>* firstBuffer;
    Array2D<T*>* secondBuffer;
    int offsetX = 0, offsetZ = 0;
    
    std::function<T*(int, int)> outCallback;

    bool in_bounds(int x, int z) const {
        return x >= 0 && z >= 0 && x < size && z < size;
    }
};