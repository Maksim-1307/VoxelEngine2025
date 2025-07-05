#pragma once

#include <iostream>
#include <stdexcept>

/* linearized 3d array  */

template <typename T>
class Array2D
{
public:
    Array2D(const size_t sizeX, const size_t sizeY) : 
        X(sizeX),
        Y(sizeY),
        data(new T[sizeX * sizeY]())
    {
    }
    ~Array2D()
    {
        delete[] data;
    }
    Array2D(const Array2D &) = delete;
    Array2D &operator=(const Array2D &) = delete;

    T get(size_t x, size_t y) const
    {
        check_bounds(x, y);
        return data[x * Y + y];
    }
    void set(size_t x, size_t y, T val)
    {
        check_bounds(x, y);
        data[x * Y + y] = val;
    }
    size_t getSizeX() const { return X; }
    size_t getSizeY() const { return Y; }

private:
    void check_bounds(size_t x, size_t y) const
    {
        if (x >= X || y >= Y)
        {
            throw std::out_of_range("ERROR: Array2D index out of bounds");
        }
    }

    const size_t X;
    const size_t Y;
    T *data;
};