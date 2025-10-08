#pragma once 

#include <iostream>
#include <vector>
#include <glm/ext.hpp>

class Hitbox {
public:
    Hitbox(glm::vec3 position, glm::vec3 size) {;
        this->halfSize = size / 2.0f;
        // position of the center of the bottom edge
        this->position = position + halfSize +halfSize.y;
        this->velocity = glm::vec3(0.0f);
        Hitbox::hitboxes.push_back(this);
    }

    bool isGrounded = false;
    float gravityFactor = 1.5f;

    glm::vec3 position;
    glm::vec3 halfSize;
    glm::vec3 velocity;

    static inline std::vector<Hitbox*> hitboxes;
};
