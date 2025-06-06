#include "Font.hpp"

Font::Font(std::string path)
{
    _texture = new Texture(path + "/font.png", true);
    std::string json = utils::read_file(path + "/font.json");
    parse_data(json);
}

void Font::parse_data(std::string jsonString)
{
    json data = json::parse(jsonString);

    uint _lineHeight = (uint)data["line-height"];
    uint _imageWidth = (uint)data["image-width"];
    uint _imageHeight = (uint)data["image-height"];

    uint lnum = 0;
    for (auto line : data["lines"]){
        uint offset = 0;
        for (auto& [key, val] : line.items()){
            char c = key[0];
            uint w0 = (uint)val;
            _characters[c] = {
                glm::ivec2(offset, lnum * _lineHeight),
                glm::ivec2(w0, _lineHeight)
            };
            offset += w0;
        }
        lnum += 1;
    }
}

CharacterData Font::get_character(char c){
    return _characters[c]; 
}

