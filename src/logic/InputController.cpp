#include "InputController.hpp"
#include "State.hpp"
#include "src/voxels/Block.hpp"

InputController::InputController(Camera &camera, Window &window)
{
    this->pCamera = &camera;
    this->pWindow = &window;
    this->pMouse = new Mouse(pWindow, pCamera);
}

void InputController::onPress(GLuint key, std::function<void()> callback) {
    input_callbacks.push_back({callback, key, InputType::KEY_PRESS, {0.0f, false}});
}
void InputController::whileHolding(GLuint key, std::function<void()> callback) {
    input_callbacks.push_back({callback, key, InputType::KEY_HOLD, {0.0f, false}});
}

void InputController::update(float deltaTime)
{

    for (auto &callback : input_callbacks) {
        switch (callback.type) {
            case InputType::KEY_PRESS:
                if (glfwGetKey(pWindow->get_glfw_window(), callback.key) == GLFW_PRESS) {
                    callback.cache.timePressed += deltaTime;
                    if (!callback.cache.prev) callback.key_callback();
                    callback.cache.prev = true;
                } else {
                    callback.cache.prev = false;
                    callback.cache.timePressed = 0.0f;
                }
                break;
            case InputType::KEY_HOLD:
                if (glfwGetKey(pWindow->get_glfw_window(), callback.key) == GLFW_PRESS) {
                    callback.cache.timePressed += deltaTime;
                    callback.cache.prev = true;
                    callback.key_callback();
                } else {
                    callback.cache.prev = false;
                    callback.cache.timePressed = 0.0f;
                }
                break;
            default:
                break;
            }
    }

    float speed = 10.0f;
    // 
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
    //
        
    if (State::MOUSE_CONTROL) {
        pMouse->update(deltaTime);
    } else {
        if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_RIGHT))
            pCamera->rotate(-speed * deltaTime * 10, 0, 0);
        if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_LEFT))
            pCamera->rotate(speed * deltaTime * 10, 0, 0);
        if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_UP))
            pCamera->rotate(0, speed * deltaTime * 10, 0);
        if (glfwGetKey(pWindow->get_glfw_window(), GLFW_KEY_DOWN))
            pCamera->rotate(0, -speed * deltaTime * 10, 0);
    }

}