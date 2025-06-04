#include "Canvas.hpp"

Canvas::Canvas(Window &window) : window(window){
    this->window = window;
}

mat4 Canvas::get_projection(){
    float ratio = window.get_ratio();
    return ortho(0.0f, static_cast<float>(window.get_width()), static_cast<float>(window.get_height()), 0.0f, -1.0f, 1.0f);
}
