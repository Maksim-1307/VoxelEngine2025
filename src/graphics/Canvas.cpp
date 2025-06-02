#include "Canvas.hpp"

Canvas::Canvas(Window &window) : window(window){
    this->window = window;
}

mat4 Canvas::get_projection(){
    float aspectRatio = window.get_ratio();
    return ortho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, -1.0f, 1.0f);
}

mat4 Canvas::get_positioning(POSITION X, int valX, POSITION Y, int valY){

    float width = static_cast<float>(window.get_width());
    float height = static_cast<float>(window.get_height());

    float normalizedX = valX / (width / 2.0f);
    float normalizedY = valY / (height / 2.0f);

    float finalX = (X == POSITION::LEFT) ? -1.0f + normalizedX : 1.0f - normalizedX;
    float finalY = (Y == POSITION::TOP) ? -1.0f + normalizedY : 1.0f - normalizedY;

    return translate(mat4(1.0f), vec3(-finalX, -finalY+1.0f, 0.0f));
}