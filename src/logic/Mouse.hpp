#pragma once
#include "src/logic/pointers.hpp"
#include "src/logic/Settings.hpp"

class Camera;
class Window;

class Mouse {
    public:
    Mouse(Window* window, Camera* camera){
        this->window = window;
        this->camera = camera;
    }
    void update(float deltaTime);
    private:
    float pressTimeLeft = 0;
    float pressTimeRight = 0;
    Window* window;
    Camera* camera;
};