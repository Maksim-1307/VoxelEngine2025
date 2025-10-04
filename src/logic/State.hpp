#pragma once

#include "Settings.hpp"

class State {
public:
    static bool MOUSE_CONTROL;
};

// Inline initialization in header (C++17+)
inline bool State::MOUSE_CONTROL = Settings::MOUSE_CONTROL;