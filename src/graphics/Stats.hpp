#pragma once
#include "Text.hpp"

class Stats {
public:
    Stats(){};

    void set(std::string prop, std::string val) {
        stats[prop] = val;
        str = "";
        for (auto pair : stats) {
            str += pair.first + ": " + pair.second + "\n";
        }
        text->update(str);
    }
    void draw() {
        text->draw();
    }

private:
    std::string str = "";
    std::map<std::string, std::string> stats;
    Text* text = new Text(str);
};