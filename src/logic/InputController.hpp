#pragma once

#include <iostream>

#include "src/graphics/Window.hpp"
#include "src/graphics/Camera.hpp"

using namespace glm;

class InputController
{
public:
    InputController(Camera &camera, Window &window);
    void update(float deltaTime);

private:
    Camera* pCamera;
    Window* pWindow;
};
