#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "ChunkPos.hpp"
#include "Array2D.hpp"
#include "Array3D.hpp"

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
    // 3D
    static std::vector<T> padding_3d(Array3D<T>* matrix, int level) {
        if (matrix->getSizeX() != matrix->getSizeY() || matrix->getSizeX() != matrix->getSizeZ()) {
            throw std::invalid_argument("Argument error: matrix must be cubic!");
        }
        int size = matrix->getSizeX();
        
        if (level < 0 || level > (size - 1) / 2) {
            throw std::invalid_argument("Invalid level: must be between 0 and (size-1)/2");
        }
        if (size % 2 != 1) {
            throw std::invalid_argument("Argument error: matrix size must be odd!");
        }
        
        int bufferLength = std::pow(size - 2 * level, 3) - std::pow(size - 2 * level - 2, 3); 
        std::vector<T> entries;
        entries.reserve(bufferLength);
        int center = (size - 1) / 2;
        int targetDistance = center - level; 
        
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int chebyshevDistance = std::max(std::abs(x - center), std::max(std::abs(y - center), std::abs(z - center)));
                    if (chebyshevDistance != targetDistance) continue; 
                    entries.emplace_back(matrix->get(x, y, z));
                }
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
        entries.emplace_back(matrix->get(x, y));
        for (int step = 0; step < radius * 2; step += 2) {
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

    static std::vector<T> in_radius_3d(Array3D<T>* matrix, int radius) {
        if (matrix->getSizeX() != matrix->getSizeY() || matrix->getSizeX() != matrix->getSizeZ()) {
            throw std::invalid_argument("Argument error: matrix must be cubic!");
        }
        int size = matrix->getSizeX();
        
        if (radius < 0 || radius > (size - 1) / 2) {
            throw std::invalid_argument("Invalid radius: must be between 0 and (size-1)/2");
        }
        if (size % 2 != 1) {
            throw std::invalid_argument("Argument error: matrix size must be odd!");
        }
        
        int totalElements = 0;
        for (int level = 0; level <= radius; level++) {
            if (level == 0) {
                totalElements += 1; 
            } else {
                totalElements += std::pow(size - 2 * level, 3) - std::pow(size - 2 * level - 2, 3);
            }
        }
        
        std::vector<T> entries;
        entries.reserve(totalElements);
        
        for (int level = 0; level <= radius; level++) {  
            std::vector<T> levelEntries = padding_3d(matrix, level);
            entries.insert(entries.end(), levelEntries.begin(), levelEntries.end());
        }

        return entries;
    }

    // from edge to center
    static std::vector<T> to_center(Array2D<T>* matrix, int level);
    // static std::vector<T> from_center(Array2D<T>* matrix);
};