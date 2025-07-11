#pragma once

#include "Array3D.hpp"

template <class T>
class AreaMap3D
{
public:
    AreaMap3D(int radius)
    {
        size = radius * 2;
        firstBuffer = new Array3D<T*>(size, size, size);
        secondBuffer = new Array3D<T *>(size, size, size);
    }
    T* get(int x, int y, int z){
        return firstBuffer->get(x+size/2 - offsetX, y+size/2 - offsetY, z+size/2 - offsetZ);
    }
    T *require(int x, int y, int z);
    void set_out_callback(std::function<T*(int, int, int)> callback){
        this->outCallback = callback;
    }
    T** get_data() const {
        return this->firstBuffer->get_data();
    }
    void fill(){
        for (int x = 0; x < size; x++)
        {
            for (int y = 0; y < size; y++)
            {
                for (int z = 0; z < size; z++)
                {
                    secondBuffer->set(x, y, z, outCallback(x - size / 2, y - size / 2, z - size / 2));
                }
            }
        }
        std::swap(firstBuffer, secondBuffer);
    }
    void translate(int dX, int dY, int dZ){
        if (dX == 0 && dY == 0 && dZ == 0) return;
        for (int x = 0; x < size; x++)
        {
            for (int y = 0; y < size; y++)
            {
                for (int z = 0; z < size; z++)
                {
                    if (in_bounds(x + dX, y + dY, z + dZ)){
                        secondBuffer->set(x, y, z, firstBuffer->get(x + dX, y + dY, z + dZ));
                    } else {
                        // secondBuffer->set(x, y, z, firstBuffer->get(x, y, z));
                        secondBuffer->set(x, y, z, outCallback(x + dX - size / 2, y + dY - size / 2, z + dZ - size / 2));
                    }
                }
            }
        }
        offsetX += dX;
        offsetY += dY;
        offsetZ += dZ;
        std::swap(firstBuffer, secondBuffer);
    }
    int get_size(){
        return this->size;
    }
    bool is_inside(int x, int y, int z){
        x = x - offsetX + size / 2;
        y = y - offsetY + size / 2;
        z = z - offsetZ + size / 2;
        return x > 0 && y > 0 && z > 0 && x < size-1 && y < size-1 && z < size-1;
    }

private:
    Array3D<T*>* firstBuffer;
    Array3D<T*>* secondBuffer;
    int offsetX = 0, offsetY = 0, offsetZ = 0;
    int size;
    bool set(int x, int y, int z, T val);
    bool resize(int newSize);
    std::function<T*(int, int, int)> outCallback;
    bool in_bounds(int x, int y, int z){
        return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
    }
};

/*

#pragma once

#include <vector>
#include <stdexcept>
#include <functional>
#include <glm/glm.hpp>

namespace util {

    template<class T, typename TCoord=int>
    class AreaMap2D {
    public:
        using OutCallback = std::function<void(TCoord, TCoord, const T&)>;
    private:
        TCoord offsetX = 0, offsetY = 0;
        TCoord sizeX, sizeY;
        std::vector<T> firstBuffer;
        std::vector<T> secondBuffer;
        OutCallback outCallback;

        size_t valuesCount = 0;

        void translate(TCoord dx, TCoord dy) {
            if (dx == 0 && dy == 0) {
                return;
            }
            std::fill(secondBuffer.begin(), secondBuffer.end(), T{});
            for (TCoord y = 0; y < sizeY; y++) {
                for (TCoord x = 0; x < sizeX; x++) {
                    auto value = std::move(firstBuffer[y * sizeX + x]);
                    auto nx = x - dx;
                    auto ny = y - dy;
                    if (value == T{}) {
                        continue;
                    }
                    if (nx < 0 || ny < 0 || nx >= sizeX || ny >= sizeY) {
                        if (outCallback) {
                            outCallback(x + offsetX, y + offsetY, value);
                        }
                        valuesCount--;
                        continue;
                    }
                    secondBuffer[ny * sizeX + nx] = std::move(value);
                }
            }
            std::swap(firstBuffer, secondBuffer);
            offsetX += dx;
            offsetY += dy;
        }
    public:
        AreaMap2D(TCoord width, TCoord height)
            : sizeX(width), sizeY(height),
              firstBuffer(width * height), secondBuffer(width * height) {
        }

        const T* getIf(TCoord x, TCoord y) const {
            auto lx = x - offsetX;
            auto ly = y - offsetY;
            if (lx < 0 || ly < 0 || lx >= sizeX || ly >= sizeY) {
                return nullptr;
            }
            return &firstBuffer[ly * sizeX + lx];
        }

        T get(TCoord x, TCoord y) const {
            auto lx = x - offsetX;
            auto ly = y - offsetY;
            if (lx < 0 || ly < 0 || lx >= sizeX || ly >= sizeY) {
                return T{};
            }
            return firstBuffer[ly * sizeX + lx];
        }

        T get(TCoord x, TCoord y, const T& def) const {
            if (auto ptr = getIf(x, y)) {
                const auto& value = *ptr;
                if (value == T{}) {
                    return def;
                }
                return value;
            }
            return def;
        }

        bool isInside(TCoord x, TCoord y) const {
            auto lx = x - offsetX;
            auto ly = y - offsetY;
            return !(lx < 0 || ly < 0 || lx >= sizeX || ly >= sizeY);
        }

        const T& require(TCoord x, TCoord y) const {
            auto lx = x - offsetX;
            auto ly = y - offsetY;
            if (lx < 0 || ly < 0 || lx >= sizeX || ly >= sizeY) {
                throw std::invalid_argument("position is out of window");
            }
            return firstBuffer[ly * sizeX + lx];
        }

        bool set(TCoord x, TCoord y, T value) {
            auto lx = x - offsetX;
            auto ly = y - offsetY;
            if (lx < 0 || ly < 0 || lx >= sizeX || ly >= sizeY) {
                return false;
            }
            auto& element = firstBuffer[ly * sizeX + lx];
            if (value && !element) {
                valuesCount++;
            }
            if (element && !value) {
                valuesCount--;
            }
            element = std::move(value);
            return true;
        }

        void setOutCallback(const OutCallback& callback) {
            outCallback = callback;
        }

        void resize(TCoord newSizeX, TCoord newSizeY) {
            if (newSizeX < sizeX) {
                TCoord delta = sizeX - newSizeX;
                translate(delta / 2, 0);
                translate(-delta, 0);
                translate(delta, 0);
            }
            if (newSizeY < sizeY) {
                TCoord delta = sizeY - newSizeY;
                translate(0, delta / 2);
                translate(0, -delta);
                translate(0, delta);
            }
            const TCoord newVolume = newSizeX * newSizeY;
            std::vector<T> newFirstBuffer(newVolume);
            std::vector<T> newSecondBuffer(newVolume);
            for (TCoord y = 0; y < sizeY && y < newSizeY; y++) {
                for (TCoord x = 0; x < sizeX && x < newSizeX; x++) {
                    newFirstBuffer[y * newSizeX + x] = firstBuffer[y * sizeX + x];
                }
            }
            sizeX = newSizeX;
            sizeY = newSizeY;
            firstBuffer = std::move(newFirstBuffer);
            secondBuffer = std::move(newSecondBuffer);
        }

        void setCenter(TCoord centerX, TCoord centerY) {
            auto deltaX = centerX - (offsetX + sizeX / 2);
            auto deltaY = centerY - (offsetY + sizeY / 2);
            if (deltaX | deltaY) {
                translate(deltaX, deltaY);
            }
        }

        void clear() {
            for (TCoord y = 0; y < sizeY; y++) {
                for (TCoord x = 0; x < sizeX; x++) {
                    auto i = y * sizeX + x;
                    auto value = firstBuffer[i];
                    firstBuffer[i] = {};
                    if (outCallback && value != T {}) {
                        outCallback(x + offsetX, y + offsetY, value);
                    }
                }
            }
            valuesCount = 0;
        }

        TCoord getOffsetX() const {
            return offsetX;
        }

        TCoord getOffsetY() const {
            return offsetY;
        }

        TCoord getWidth() const {
            return sizeX;
        }

        TCoord getHeight() const {
            return sizeY;
        }

        const std::vector<T>& getBuffer() const {
            return firstBuffer;
        }

        size_t count() const {
            return valuesCount;
        }

        TCoord area() const {
            return sizeX * sizeY;
        }
    };
}
*/