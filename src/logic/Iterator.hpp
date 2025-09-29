#pragma once

#include <iostream>
#include <vector>
#include "ChunkPos.hpp"
#include "Array2D.hpp"

/* returns the entries in a matrix in a specified order */

template <typename T>
class Iterator {
public:

    // only padding elements. 0 level - the farthest
    static std::vector<T> padding(Array2D<T>* matrix, int level) {
        if (matrix->getSizeX() != matrix->getSizeY()) {
            throw std::invalid_argument("Argument error: matrix must be square!");
        }
        int size = matrix->getSizeX();
        int bufferLength = (size - 2 * level) * 4 - 4;
        std::vector<T> entries;
        entries.reserve(bufferLength);
        int step = size - 2 * level - 1;
        for (int x = level; x < size - level; x += step) {
            for (int y = level; y < size - level; y ++) {
                entries.emplace_back(matrix->get(x, y));
            }
        }   
        for (int y = level; y < size - level; y += step) {
            for (int x = level; x < size - level; x ++) {
                entries.emplace_back(matrix->get(x, y));
            }
        }
        return entries;
    }

    // from the center to the edge
    static std::vector<T> from_center(Array2D<T>* matrix);

    // spiral bypass of elemetns in a given radius
    static std::vector<T> in_radius(Array2D<T>* matrix, int radius) {
        if (matrix->getSizeX() != matrix->getSizeY()) {
            throw std::invalid_argument("Argument error: matrix must be square!");
        }
        int size = matrix->getSizeX();
        if (size % 2 != 1) {
            throw std::invalid_argument("Argument error: matrix size must be odd!");
        }
        std::vector<int> directions {
            1, 0,
            0, -1,
           -1, 0,
            0, 1
        };
        std::vector<T> entries;
        int centerX = size / 2;
        int centerY = size / 2;
        int x = centerX;
        int y = centerY;
        for (int step = 0; step < size; step += 2) {
            for (int dir = 0; dir < 4; dir++) {
                int dx = directions[dir*2];
                int dy = directions[dir*2+1];
                for (int _ = 0; _ < step; _++) {
                    x += dx;
                    y += dy;
                    entries.emplace_back(matrix->get(x, y));
                }
            }
            centerX -= 1;
            centerY += 1;
            x = centerX;
            y = centerY;
        }
        return entries;
    }
    // from edge to center
    static std::vector<T> to_center(Array2D<T>* matrix, int level);
    // static std::vector<T> from_center(Array2D<T>* matrix);
};