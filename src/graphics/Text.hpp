#pragma once

#include <iostream>
#include "Font.hpp"

class Text
{
public:
    Text(std::string &text);
    Mesh *get_mesh();
    void update(std::string text);
    void draw();
    Font *get_font()
    {
        return _font;
    }

private:
    void clear_data();
    void add_character(char c);
    void vertex(float x, float y, float u, float v);
    void index(uint a, uint b, uint c, uint d, uint e, uint f);
    std::vector<float> _vertices;
    std::vector<uint> _indices;
    uint _indexOffset = 0;
    Font *_font;
    Mesh *_mesh;
    std::string _text;
    MeshRenderer *_renderer;
    glm::ivec2 _offset = glm::ivec2(0, 0);
};