#include "utils.hpp"

std::string utils::read_file(std::string path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Error! The file '" + path + "' doesn't exist.");
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void utils::set_current_directory(char **argv)
{
    std::string current_path(argv[0]);
    int slashid = current_path.find_last_of("/");
    current_path = current_path.substr(0, slashid + 1);
    std::filesystem::current_path(current_path);
}