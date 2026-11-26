#include "Mouse.hpp"
#include "src/graphics/Camera.hpp"
#include "src/graphics/Window.hpp"
#include "src/physics/Terrain.hpp"
#include "src/Engine.hpp"
#include "src/logic/State.hpp"

#define MAX_MOUSE_OFFSET 100

void Mouse::update(float deltaTime) {
    if (!Settings::MOUSE_CONTROL) return;
    double xpos, ypos;
    deltaTime = std::min(deltaTime, 0.01f);
    glfwGetCursorPos(window->get_glfw_window(), &xpos, &ypos);
    double xoffset = window->get_width() / 2  - xpos;
    double yoffset = window->get_height() / 2 - ypos;
    if (std::abs(xoffset) > MAX_MOUSE_OFFSET) 
        xoffset = 0;
    if (std::abs(yoffset) > MAX_MOUSE_OFFSET)
        yoffset = 0;
    glfwSetCursorPos(window->get_glfw_window(), window->get_width()/2, window->get_height()/2);
    float sensifity = Settings::MOUSE_SENSITIVITY;
    camera->rotate(xoffset * deltaTime * sensifity, yoffset * deltaTime * sensifity, 0);

    if (glfwGetMouseButton(window->get_glfw_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        if (pressTimeLeft == 0 || pressTimeLeft > Settings::BLOCK_BREAKING_DELAY) {
            RaycastResult result = Engine::pTerrain->raycast(camera->position, camera->front, 15.0f);
            if (result.hit) {
                int bx = std::floor(result.position.x);
                int by = std::floor(result.position.y);
                int bz = std::floor(result.position.z);
                Engine::pVoxelStorage->set_voxel_soft(bx, by, bz, {0, 0});
                Engine::pLighting->onBlockSet(bx, by, bz, 0);
            }
            pressTimeLeft = 0;
        }
        pressTimeLeft += deltaTime;
    } else {
        pressTimeLeft = 0;
    }
    if (glfwGetMouseButton(window->get_glfw_window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        if (pressTimeRight == 0 || pressTimeRight > Settings::BLOCK_PLACIND_DELAY) {
            RaycastResult result = Engine::pTerrain->raycast(camera->position, camera->front, 15.0f, false);
            if (result.hit) {
                int bx = std::floor(result.position.x);
                int by = std::floor(result.position.y);
                int bz = std::floor(result.position.z);
                // Temporary for snow (КОСТЫЛЬ, переделать)
                if (State::PLACING_VOXEL.id == 14) {
                    voxel placing_in_vox = Engine::pVoxelStorage->get_voxel(bx, by, bz);
                    if (placing_in_vox.id == 14) {
                        uint8_t snow_lvl = placing_in_vox.state;
                        snow_lvl += 1;
                        voxel updated_snow = {
                            placing_in_vox.id, 
                            snow_lvl
                        };
                        Engine::pVoxelStorage->set_voxel_soft(bx, by, bz, updated_snow);
                        Engine::pLighting->onBlockSet(bx, by, bz, State::PLACING_VOXEL.id);
                    } else {
                        Engine::pVoxelStorage->set_voxel_soft(bx, by, bz, State::PLACING_VOXEL);
                    }
                } else {
                //
                Engine::pVoxelStorage->set_voxel_soft(bx, by, bz, State::PLACING_VOXEL);
                }
                Engine::pLighting->onBlockSet(bx, by, bz, State::PLACING_VOXEL.id);
            }
            pressTimeRight = 0;
        }
        pressTimeRight += deltaTime;
    } else {
        pressTimeRight = 0;
    }
}