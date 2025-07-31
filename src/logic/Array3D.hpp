#pragma once

#include <iostream>
#include <stdexcept>

/* linearized 3d array  */

template <typename T>
class Array3D {
public:
    Array3D(const size_t sizeX, const size_t sizeY, const size_t sizeZ) : 
        X(sizeX), 
        Y(sizeY), 
        Z(sizeZ), 
        data(new T[sizeX * sizeY * sizeZ]()) 
    {}
    ~Array3D(){
        delete[] data;
    }
    Array3D(const Array3D &) = delete;
    Array3D &operator=(const Array3D &) = delete;

    T get(size_t x, size_t y, size_t z) const {
        check_bounds(x, y, z);
        return data[x * (Y * Z) + y * Z + z];
    }
    T* get_data() const {
        return data;
    }
    void set(size_t x, size_t y, size_t z, T val){
        check_bounds(x, y, z);
        data[x * (Y * Z) + y * Z + z] = val;
    }
    size_t getSizeX() const { return X; }
    size_t getSizeY() const { return Y; }
    size_t getSizeZ() const { return Z; }

private:

    void check_bounds(size_t x, size_t y, size_t z) const {
        if (x >= X || y >= Y || z >= Z || x < 0 || y < 0 || z < 0)
        {
            throw std::out_of_range("ERROR: Array3D index out of bounds");
        }
    }

    const size_t X;
    const size_t Y;
    const size_t Z;
    T* data;
};