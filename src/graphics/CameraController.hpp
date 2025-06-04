#pragma once

#include <iostream>

#include "Window.hpp"
#include "Camera.hpp"

using namespace glm;

class CameraController
{
public:
    CameraController(Camera &camera, Window &window);
    void update(float deltaTime);

private:
    Camera* pCamera;
    Window* pWindow;
};
