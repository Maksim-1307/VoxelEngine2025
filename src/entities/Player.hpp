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
        this->prevPosition = position;
    };
    ~Player() {};

    void jump() {
        if (this->hitbox->isGrounded && !this->hitbox->isFloating) {
            this->hitbox->velocity.y = 12.0f;
        }
    }
    void move_forward() {
        movingDirections.push_back(glm::vec3(1.0f, 0, 0));
    }
    void move_backward() {
        movingDirections.push_back(glm::vec3(-1.0f, 0, 0));
    }
    void move_right() {
        movingDirections.push_back(glm::vec3(0, 0, 1.0f));
    }
    void move_left() {
        movingDirections.push_back(glm::vec3(0, 0, -1.0f));
    }
    void move_up() {
        if (this->hitbox->isFloating) movingDirections.push_back(glm::vec3(0, 1.0f, 0));
    }
    void move_down() {
        if (this->hitbox->isFloating) movingDirections.push_back(glm::vec3(0, -1.0f, 0));
    }

    void update() {
        glm::vec3 forward = camera->front;
        glm::vec3 right = camera->right;
        glm::vec3 up = camera->absoluteUp;
        glm::vec3 direction = this->finalDirection();
        
        if (glm::length(direction) > 0.001f) {
            if (!this->hitbox->isFloating) {
                direction.y = 0;
            }
            direction = forward * direction.x + up * direction.y + right * direction.z;
            // direction.y = 0;
            direction = glm::normalize(direction);
            this->hitbox->velocity.x = direction.x * Settings::PLAYER_SPEED;
            this->hitbox->velocity.z = direction.z * Settings::PLAYER_SPEED;
            if (this->hitbox->isFloating)
                this->hitbox->velocity.y = direction.y * Settings::PLAYER_SPEED;
        } else {
            this->hitbox->velocity.x = 0.0f;
            this->hitbox->velocity.z = 0.0f;
        }

        movingDirections.clear();
    }

// private:
    // X+ - forward, Y+ - right
    std::vector<glm::vec3> movingDirections = {};
    glm::vec3 position;
    glm::vec3 prevPosition;
    Camera* camera;
    uptr<Hitbox> hitbox;
private:
    private:
    glm::vec3 finalDirection () {
        glm::vec3 sum(0.0f);
        for (auto direction : movingDirections) {
            sum += direction;   
        }
        
        if (glm::length(sum) < 0.001f) {
            return glm::vec3(0.0f); 
        }
        
        return glm::normalize(sum);
    }
};
