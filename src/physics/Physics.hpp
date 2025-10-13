#pragma once

#include "Hitbox.hpp"

enum Axis {
    X, Y, Z
};

class Physics {
public:
    Physics() {};
    ~Physics() {};

    inline static glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
    static void collisions(Hitbox& hitbox, float deltaTime);
    static void checkDir(Axis dir, Hitbox& hitbox, float deltaTime);
    static void step(float deltaTime);
    static bool isColliding(const glm::vec3& center, const glm::vec3& halfSize);
    static float resolveAxis(Hitbox& hitbox, const glm::vec3& movement, int axis);
    static glm::vec3 resolveCollisions(Hitbox& hitbox, const glm::vec3& movement);

};