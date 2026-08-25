#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    static inline glm::vec3 get_sun_vector() {
        float dayTime = Time::get_day_time();
        float rotationAngle = dayTime * 2 * glm::pi<float>();; // from 0 to 2 PI
        glm::vec3 sunVector = glm::vec3(0.0f, 0.0f, 1.0f); // initial vector, pointing to Z+ (east)
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 rotation = glm::rotate(model, rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::normalize(glm::vec3(rotation * glm::vec4(sunVector, 0.0f)));
    }
private:
    static glm::vec3 sunVector;
    static glm::vec3 skyColor;
    static constexpr glm::vec3 dayColor{0.67f, 0.84f, 0.9f};
};