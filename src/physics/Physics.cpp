#include "Physics.hpp"
#include "src/Engine.hpp"

void Physics::step(float deltaTime) {
    for (auto hitbox : Hitbox::hitboxes) {
        hitbox->velocity += gravity * hitbox->gravityFactor * deltaTime;
        glm::vec3 movement = hitbox->velocity * deltaTime;
        movement = resolveCollisions(*hitbox, movement);
        hitbox->position += movement;

        // Push hitbox out of terrain if still colliding after movement
        if (isColliding(hitbox->position, hitbox->halfSize)) {
            const float pushStep = 0.05f;
            for (int i = 0; i < 6; i++) {
                glm::vec3 dir(0);
                if (i < 2) dir.x = (i == 0 ? 1 : -1) * pushStep;
                else if (i < 4) dir.y = (i == 2 ? 1 : -1) * pushStep;
                else dir.z = (i == 4 ? 1 : -1) * pushStep;

                glm::vec3 testPos = hitbox->position + dir;
                if (!isColliding(testPos, hitbox->halfSize)) {
                    hitbox->position = testPos;
                    break;
                }
            }
        }
    }
}

glm::vec3 Physics::resolveCollisions(Hitbox& hitbox, const glm::vec3& movement) {
    glm::vec3 result = movement;

    result.x = resolveAxis(hitbox, result, 0);
    result.z = resolveAxis(hitbox, result, 2);
    result.y = resolveAxis(hitbox, result, 1);

    // Binary search fallback for corner cases:
    // per-axis checks allow X and Z individually, but their combination
    // with Y pushes the hitbox corner into the terrain.
    // Search along `result` to find the nearest non-colliding position.
    if (isColliding(hitbox.position + result, hitbox.halfSize)) {
        glm::vec3 safe(0);
        glm::vec3 unsafe = result;
        for (int i = 0; i < 6; i++) {
            glm::vec3 mid = (safe + unsafe) * 0.5f;
            if (isColliding(hitbox.position + mid, hitbox.halfSize)) {
                unsafe = mid;
            } else {
                safe = mid;
            }
        }
        result = safe;
    }

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