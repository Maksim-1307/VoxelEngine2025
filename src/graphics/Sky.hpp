#pragma once

#include <glm/glm.hpp>
#include "src/time/Time.hpp"

class Sky {
public:
    static void update();
    static inline glm::vec3 get_color() {
        return skyColor;
    }
    static inline float get_sky_brightness() {
        float dayTime = Time::get_day_time();
        if (dayTime < 0.25f) {
            return dayTime * 4.0f; // 0.0 to 1.0
        } else if (dayTime < 0.5f) {
            return 1.0f; // full brightness
        } else if (dayTime < 0.75f) {
            return (0.75f - dayTime) * 4.0f; // 1.0 to 0.0
        } else {
            return 0.0f; // night
        }
    }
private:
    static glm::vec3 skyColor;
    static constexpr glm::vec3 dayColor{0.67f, 0.84f, 0.9f};
};