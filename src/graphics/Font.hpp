#pragma once

#include <iostream>
#include <vector>

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"
#include "MeshRenderer.hpp"
#include "Texture.hpp"

struct CharacterData {
    glm::ivec2 uvPos;
    glm::ivec2 size;
};

class Font {
public:
    Font(std::string path);
    Font(){};
    glm::vec2 get_character_uv(char c);
    CharacterData get_character(char c);
    uint get_width() {
        return _imageWidth;
    }
    uint get_height(){
        return _imageHeight;
    }
    uint get_line_height(){
        return _lineHeight;
    }
private:
    uint _lineHeight = 11;
    uint _imageWidth = 512;
    uint _imageHeight = 512;
};

class Text {
public:
    Text(std::string& text);
    Mesh* get_mesh();
    void update(std::string text);
    void draw();
    Texture* get_texture(){
        return _texture;
    }
private:
    void add_character(char c);
    void vertex(float x, float y, float u, float v);
    void index(uint a, uint b, uint c, uint d, uint e, uint f);
    Texture* _texture;
    std::vector<float> _vertices;
    std::vector<uint> _indices;
    uint _indexOffset = 0;
    Font* _font;
    Mesh* _mesh;
    std::string _text;
    MeshRenderer* _renderer;
    glm::ivec2 _offset= glm::ivec2(0, 0);
};