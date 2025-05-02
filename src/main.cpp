#include <iostream>

#include "Engine.hpp"
#include "logic/utils.hpp"

int main(int argc, char **argv)
{

    utils::set_current_directory(argv);

    Engine engine;
    engine.init();
    engine.game_loop();
    
    return 0;
}
