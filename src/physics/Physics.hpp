#pragma once

#include "Hitbox.hpp"

class Physics {
public:
    Physics() {};
    ~Physics() {};

    inline static glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
    static void collisions(Hitbox& hitbox, float deltaTime);
    static void step(float deltaTime);
};