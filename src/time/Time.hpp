#pragma once

#include <iostream>
#include "src/logic/Settings.hpp"
#include "RandomTick.hpp"

class Time {
public:
    static void tick() {
        Time::ticksTotal++;
        RandomTick::tick();
    }
    static inline float get_day_time() {
        return (float)(ticksTotal % Settings::DAY_LENGTH) / Settings::DAY_LENGTH;
    }
private:
    inline static long long ticksTotal = 0;
};