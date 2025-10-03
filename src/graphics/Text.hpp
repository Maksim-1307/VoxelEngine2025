#pragma once

#include <iostream>
#include "src/logic/pointers.hpp"
#include "Font.hpp"
#include "src/logic/pointers.hpp"

class Text
{
public:
    Text(std::string &text);
    sptr<Mesh> get_mesh();
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
    sptr<Mesh> _mesh;
    uptr<MeshRenderer> _renderer;
    std::string _text;
    glm::ivec2 _offset = glm::ivec2(0, 0);
};