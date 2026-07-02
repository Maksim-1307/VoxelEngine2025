#pragma once

#include <iostream>
#include "include/pcg_random.hpp"

constexpr int UPDATES_PER_TICK = 100;

class RandomTick {
public:
    static void tick();
private:
    static pcg32 rng;
};