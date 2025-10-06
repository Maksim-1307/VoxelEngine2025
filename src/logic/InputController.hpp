#pragma once

#include <iostream>

#include "src/logic/Settings.hpp"
#include "src/graphics/Window.hpp"
#include "src/graphics/Camera.hpp"
#include "src/logic/Mouse.hpp"

using namespace glm;

enum InputType {
    KEY_PRESS,
    KEY_HOLD
};

struct InputCache {
    // time since key was pressed
    float timePressed = 0.0f;
    // was pressed at previous update
    bool prev = false;
};

struct InputCallback {
    std::function<void()> key_callback;
    GLuint key;
    InputType type;
    InputCache cache;
};

class InputController
{
public:
    InputController(Camera &camera, Window &window);
    void update(float deltaTime);
    void onPress(GLuint key, std::function<void()> callback);
    void whileHolding(GLuint key, std::function<void()> callback);

private:
    std::vector<InputCallback> input_callbacks;
    Camera* pCamera;
    Window* pWindow;
    Mouse* pMouse;
};
