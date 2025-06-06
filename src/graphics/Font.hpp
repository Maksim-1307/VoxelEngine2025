#pragma once

#include <iostream>
#include <vector>
#include <map>

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/json.hpp"

#include "Mesh.hpp"
#include "MeshRenderer.hpp"
#include "Texture.hpp"
#include "src/logic/utils.hpp"

using json = nlohmann::json;

struct CharacterData {
    glm::ivec2 uvPos;
    glm::ivec2 size;
};

class Font {
public:
    Font(std::string path);
    Font(){};
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
    Texture *get_texture()
    {
        return _texture;
    }
private:
    void parse_data(std::string jsonString);
    std::map<char, CharacterData> _characters;
    uint _lineHeight = 11;
    uint _imageWidth = 512;
    uint _imageHeight = 512;
    Texture *_texture;
};
