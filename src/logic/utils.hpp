#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace utils{

    std::string read_file(std::string path);
    void set_current_directory(char** argv);

}

