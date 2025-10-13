#include "Physics.hpp"
#include "src/Engine.hpp"

#define E 0.01f

void Physics::step(float deltaTime) {
    for (auto hitbox : Hitbox::hitboxes) {
        // applying forces
        hitbox->velocity += gravity * hitbox->gravityFactor * deltaTime;
        // calculating movement
        glm::vec3 movement = hitbox->velocity * deltaTime;
        // then correcting the movement accorging to physical iteractions with terrain
        movement = resolveCollisions(*hitbox, movement);
        hitbox->position += movement;
    }
}

glm::vec3 Physics::resolveCollisions(Hitbox& hitbox, const glm::vec3& movement) {
    glm::vec3 result = movement;

    // for ecah axis separately
    result.x = resolveAxis(hitbox, result, 0);
    result.z = resolveAxis(hitbox, result, 2);
    result.y = resolveAxis(hitbox, result, 1);
    
    return result;
}

float Physics::resolveAxis(Hitbox& hitbox, const glm::vec3& movement, int axis) {
    float proposedMove = axis == 0 ? movement.x : (axis == 1 ? movement.y : movement.z);
    if (proposedMove == 0) return 0;
    
    glm::vec3 testPos = hitbox.position;
    if (axis == 0) testPos.x += proposedMove;
    else if (axis == 1) testPos.y += proposedMove;
    else testPos.z += proposedMove;
    
    if (!isColliding(testPos, hitbox.halfSize)) {
        return proposedMove;
    }
    
    hitbox.isGrounded = (axis == 1 && proposedMove < 0);
    
    if (axis == 0) hitbox.velocity.x = 0;
    else if (axis == 1) hitbox.velocity.y = 0;
    else hitbox.velocity.z = 0;
    
    return 0;
}

bool Physics::isColliding(const glm::vec3& center, const glm::vec3& halfSize) {
    glm::vec3 min = center - halfSize;
    glm::vec3 max = center + halfSize;

    float dX = (max.x - min.x) / 10;
    float dY = (max.y - min.y) / 10;
    float dZ = (max.z - min.z) / 10;

    for (float x = min.x; x <= max.x; x += dX) {
        for (float y = min.y; y <= max.y; y += dY) {
            for (float z = min.z; z <= max.z; z += dZ) {
                if (Engine::pTerrain->is_obstacle_at(x, y, z)) {
                    return true;
                }
            }
        }
    }

    return false;
}