#include "Sky.hpp"
#include "src/Engine.hpp"

glm::vec3 Sky::skyColor = Sky::dayColor;

void Sky::update() {
    skyColor = dayColor * get_sky_brightness();
    Engine::pWindow->set_clear_color(skyColor);
}