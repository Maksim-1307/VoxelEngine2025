#include "Text.hpp"

Text::Text(std::string &text)
{
    _text = text;
    _font = new Font("res/font");
    update(text);
}

void Text::update(std::string text)
{
    clear_data();
    _text = text;
    for (char c : _text)
    {
        add_character(c);
    }
}

void Text::draw()
{
    if (_renderer == nullptr)
    {
        if (_mesh == nullptr)
        {
            get_mesh();
        }
        _renderer = new MeshRenderer(_mesh, MeshType::SPRITE2D);
    }
    _renderer->draw();
}

void Text::clear_data()
{
    delete _renderer;
    delete _mesh;
    _renderer = nullptr;
    _mesh = nullptr;
    _vertices.clear();
    _indices.clear();
    _indexOffset = 0;
    _offset = glm::ivec2(0, 0);
    _text = "";
}

void Text::add_character(char c)
{

    CharacterData ch = _font->get_character(c);

    uint texWidth = _font->get_width();
    uint texHeight = _font->get_height();
    uint lh = _font->get_line_height();
    int w0 = ch.size.x;
    int posX = ch.uvPos.x;
    int posY = ch.uvPos.y;
    float fz = 2.0f;

    glm::vec2 pos = {(float)posX / (float)texWidth, (float)posY / (float)texHeight * -1};
    float w = (float)w0 / (float)texWidth;
    float h = (float)lh / (float)texHeight;

    if (c == ' ')
    {
        _offset.x += 4 * fz;
        return;
    }
    if (c == '\n')
    {
        _offset.x = 0;
        _offset.y += lh * fz;
        return;
    }

    vertex(0.0f, 0.0f, 0.0f + pos.x, 1.0f - h + pos.y);
    vertex(w0 * fz, 0.0f, w + pos.x, 1.0f - h + pos.y);
    vertex(0.0f, lh * fz, 0.0f + pos.x, 1.0f + pos.y);
    vertex(w0 * fz, lh * fz, w + pos.x, 1.0f + pos.y);

    index(0, 1, 3, 0, 3, 2);

    _offset.x += w0 * fz;
}

void Text::vertex(float x, float y, float u, float v)
{
    const float newVertex[] = {_offset.x + x, _offset.y + y, u, v};
    _vertices.insert(_vertices.end(), std::begin(newVertex), std::end(newVertex));
}

void Text::index(uint a, uint b, uint c, uint d, uint e, uint f)
{
    const uint newIndices[] = {
        _indexOffset + a,
        _indexOffset + b,
        _indexOffset + c,
        _indexOffset + d,
        _indexOffset + e,
        _indexOffset + f};
    _indices.insert(_indices.end(), std::begin(newIndices), std::end(newIndices));
    _indexOffset += 4;
}

Mesh *Text::get_mesh()
{
    _mesh = new Mesh(_vertices.data(), _vertices.size(), _indices.data(), _indices.size());
    return _mesh;
}
