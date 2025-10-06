#pragma once

#include "Settings.hpp"
#include "src/voxels/voxel.hpp"

class State {
public:
    static bool MOUSE_CONTROL;
    static voxel PLACING_VOXEL;
};

inline bool State::MOUSE_CONTROL = Settings::MOUSE_CONTROL;
inline voxel State::PLACING_VOXEL = {1, 0};