#include "InputController.hpp"

InputController::InputController(Camera &camera, Window &window)
{
    this->pCamera = &camera;
    this->pWindow = &window;
}

void InputController::update(float deltaTime)
{
    float speed = 10.0f;
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_W)) 
        pCamera->move(0, 0, -speed*deltaTime);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_A))
        pCamera->move(-speed * deltaTime, 0, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_S))
        pCamera->move(0, 0, speed * deltaTime);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_D))
        pCamera->move(speed * deltaTime, 0, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_SPACE))
        pCamera->move(0, speed * deltaTime, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_LEFT_SHIFT))
        pCamera->move(0, -speed * deltaTime, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_RIGHT))
        pCamera->rotate(-speed * deltaTime * 10, 0, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_LEFT))
        pCamera->rotate(speed * deltaTime * 10, 0, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_UP))
        pCamera->rotate(0, speed * deltaTime * 10, 0);
    if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_DOWN))
        pCamera->rotate(0, -speed * deltaTime * 10, 0);
}