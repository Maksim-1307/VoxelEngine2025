#include "Physics.hpp"
#include "src/Engine.hpp"

void Physics::step(float deltaTime) {
    for (auto hitbox : Hitbox::hitboxes) {
        hitbox->velocity += gravity * deltaTime;
        collisions(*hitbox, deltaTime);
        hitbox->position += hitbox->velocity * deltaTime;
    }
}

void Physics::collisions(Hitbox& hitbox, float deltaTime) {
    float step = 0.5f;
    glm::vec3 npos = hitbox.position + hitbox.velocity * deltaTime;
    glm::vec3 halfSize = hitbox.halfSize;
    glm::vec3 a = npos - halfSize;
    glm::vec3 b = npos + halfSize;
    
    // check for collisions for each side
    bool isFree = true;
    for (float x = a.x; x <= b.x; x += 2*halfSize.x) {
        for (float y = a.y+0.01f; y <= b.y; y += step) {
            for (float z = a.z; z <= b.z; z += step) {
                if (Engine::pTerrain->is_obstacle_at(x, y, z)) {
                    isFree = false;
                    break;
                }
            }
        }
    }
    if (!isFree) {
        hitbox.velocity.x = 0;
    }

    isFree = true;
    for (float x = a.x; x <= b.x; x += step) {
        for (float y = a.y; y <= b.y; y += 2*halfSize.y) {
            for (float z = a.z; z <= b.z; z += step) {
                if (Engine::pTerrain->is_obstacle_at(x, y, z)) {
                    isFree = false;
                    break;
                }
            }
        }
    }
    if (!isFree) {
        hitbox.velocity.y = 0;
        hitbox.isGrounded = true;
    } else {
        hitbox.isGrounded = false;
    }

    isFree = true;
    for (float x = a.x; x <= b.x; x += step) {
        for (float y = a.y+0.01f; y <= b.y; y += step) {
            for (float z = a.z; z <= b.z; z += 2*halfSize.z) {
                if (Engine::pTerrain->is_obstacle_at(x, y, z)) {
                    isFree = false;
                    break;
                }
            }
        }
    }
    if (!isFree) {
        hitbox.velocity.z = 0;
    }
}