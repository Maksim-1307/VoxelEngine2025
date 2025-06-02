#pragma once

#include <iostream>

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"

using namespace glm;

enum class POSITION
{
    LEFT,
    TOP,
    RIGHT,
    BOTTOM
};

class Canvas
{

public:
    Canvas(Window &window);

    Window &window;

    // orthographic projection (for 2d sprites)
    mat4 get_projection();

    // positioning 2d sprite in canvas (for 2d sprites)
    // center is default for each axis
    mat4 get_positioning(POSITION X, int valX, POSITION Y, int valY);
};
