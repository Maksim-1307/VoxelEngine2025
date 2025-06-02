#include "Camera.hpp"
#include <glm/ext.hpp>

Camera::Camera(Window &window) : window(window)
{

    this->window = window;
    updateVectors();
}

void Camera::updateVectors()
{

    front.x = cos(glm::radians(horizontalRot)) * cos(glm::radians(verticalRot));
    front.y = sin(glm::radians(verticalRot));
    front.z = sin(glm::radians(horizontalRot)) * cos(glm::radians(verticalRot));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, absoluteUp));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::rotate(float x, float y, float z)
{

    horizontalRot -= x;
    verticalRot += y;

    if (verticalRot < -89.99f)
    {
        verticalRot = -89.99f;
    }
    else if (verticalRot > 89.99f)
    {
        verticalRot = 89.99f;
    }

    updateVectors();
}

void Camera::move(float x, float y, float z)
{
    position = position - front * z;
    position = position + right * x;
    position = position + vec3(0, y, 0);
    updateVectors();
}

mat4 Camera::getView()
{
    return lookAt(position, position + front, up);
}

mat4 Camera::getViewFromNull()
{
    return lookAt(-front, vec3(0), vec3(0, 1, 0));
}

mat4 Camera::getProjection()
{
    return perspective(glm::radians(fov), (float)window.get_width() / (float)window.get_height(), 0.01f, 1500.0f);
}

mat4 Camera::get_orthographic_projection()
{
    float aspectRatio = window.get_ratio();
    return ortho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, -1.0f, 1.0f);
}

mat4 Camera::get_canvas_positioning(/* enum for positions */){
    
}
