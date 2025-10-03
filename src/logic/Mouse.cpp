#include "Mouse.hpp"
#include "src/graphics/Camera.hpp"
#include "src/graphics/Window.hpp"

void Mouse::update(float deltaTime) {
    if (!Settings::MOUSE_CONTROL) return;
    double xpos, ypos;
    deltaTime = std::min(deltaTime, 0.01f);
    glfwGetCursorPos(window->get_glfw_window(), &xpos, &ypos);
    double xoffset = window->get_width() / 2  - xpos;
    double yoffset = window->get_height() / 2 - ypos;
    glfwSetCursorPos(window->get_glfw_window(), window->get_width()/2, window->get_height()/2);
    float sensifity = Settings::MOUSE_SENSITIVITY;
    camera->rotate(xoffset * deltaTime * sensifity, yoffset * deltaTime * sensifity, 0);
}