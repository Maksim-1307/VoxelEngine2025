#pragma once

#include <iostream>

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"

using namespace glm;

class Camera
{

    void updateVectors();

public:
    Camera(Window &window);

    vec3 position = vec3(0, 1, 3);
    vec3 front;
    vec3 right;
    vec3 up;
    vec3 absoluteUp = vec3(0, 1, 0);
    float fov = 45.0f;

    // horizontal and vertical camera rotation
    float horizontalRot = 270.0f; // from 0 to 360
    float verticalRot = 0.0f;     // from -90 to 90

    Window &window;

    mat4 getView();

    mat4 getViewFromNull();

    mat4 getProjection();

    mat4 get_orthographic_projection();

    void move(float x, float y, float z);

    void rotate(float x, float y, float z);
};

