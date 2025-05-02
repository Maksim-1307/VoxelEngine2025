#pragma once

#include "graphics/Window.hpp"

class Engine {
    public:
        Engine();
        void init();
        void game_loop();

        static Window* pWindow;
};