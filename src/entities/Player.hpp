#pragma once

#include "src/logic/pointers.hpp"
#include "src/physics/Hitbox.hpp"
#include "src/graphics/Camera.hpp"
#include "src/logic/Settings.hpp"

#include <unordered_set>

class Player {
public: 
    Player(glm::vec3 position, Camera* camera) {
        this->position = position;
        this->camera = camera;
        this->camera->position = position + glm::vec3(0, 1.6f-0.9f, 0);
        this->hitbox = make_uptr<Hitbox>(position, glm::vec3(0.8f, 1.8f, 0.8f));
        this->hitbox->gravityFactor = 2.0f;
        this->hitbox->position = position;
    };
    ~Player() {};

    void jump() {
        if (this->hitbox->isGrounded) this->hitbox->velocity.y = 12.0f;
    }
    void move_forward() {
        movingDirections.push_back(glm::vec2(1.0f, 0));
    }
    void move_backward() {
        movingDirections.push_back(glm::vec2(-1.0f, 0));
    }
    void move_right() {
        movingDirections.push_back(glm::vec2(0, 1.0f));
    }
    void move_left() {
        movingDirections.push_back(glm::vec2(0, -1.0f));
    }

    void update() {
        camera->position = hitbox->position + glm::vec3(0, 1.6f-0.9f, 0);

        glm::vec3 forward = camera->front;
        glm::vec3 right = camera->right;
        glm::vec3 direction = this->finalDirection();
        
        if (glm::length(direction) > 0.001f) {
            direction = forward * direction.x + right * direction.z;
            direction.y = 0;
            direction = glm::normalize(direction);
            this->hitbox->velocity.x = direction.x * Settings::PLAYER_SPEED;
            this->hitbox->velocity.z = direction.z * Settings::PLAYER_SPEED;
        } else {
            this->hitbox->velocity.x = 0.0f;
            this->hitbox->velocity.z = 0.0f;
        }

        movingDirections.clear();
    }

// private:
    // X+ - forward, Y+ - right
    std::vector<glm::vec2> movingDirections = {};
    glm::vec3 position;
    Camera* camera;
    uptr<Hitbox> hitbox;
private:
    private:
    glm::vec3 finalDirection () {
        glm::vec2 sum(0.0f);
        for (auto direction : movingDirections) {
            sum += direction;   
        }
        
        if (glm::length(sum) < 0.001f) {
            return glm::vec3(0.0f); 
        }
        
        return glm::normalize(glm::vec3(sum.x, 0.0f, sum.y));
    }
};
