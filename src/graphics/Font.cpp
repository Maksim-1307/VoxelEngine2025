#include "Font.hpp"

/*
move Texture from Text to Font
separate Font and Text classes  
*/

Font::Font(std::string path)
{
}

Text::Text(std::string& text){
    _text = text;
    _font = new Font("res/font");
    _texture = new Texture("res/font/font.png", true);
    add_character('a');
    add_character('a');
    add_character('a');
    add_character('a');
    add_character('a');
}

void Text::draw(){
    if (_renderer == nullptr){
        if (_mesh == nullptr) {
            get_mesh();
        }
        _renderer = new MeshRenderer(_mesh, MeshType::SPRITE2D);
    }
    _renderer->draw();
}

CharacterData Font::get_character(char c){
    return {
        {0, 0},
        {4, 11}
    };
}

void Text::add_character(char c)
{
    CharacterData ch = _font->get_character(c);

    uint texWidth = _font->get_width();
    uint texHeight = _font->get_height();
    uint lh = _font->get_line_height();
    // int lineNum = ch.line;
    int w0 = ch.size.x;
    int posX = ch.uvPos.x;
    int posY = ch.uvPos.y;
    float fz = 2.0f;

    glm::vec2 pos = {(float)posX / (float)texWidth, (float)posY / (float)texHeight * -1};
    float w = (float)w0 / (float)texWidth;
    float h = (float)lh / (float)texHeight;

    vertex(0.0f, 0.0f, 0.0f + pos.x, 1.0f - h + pos.y);
    vertex(w0 * fz, 0.0f, w + pos.x, 1.0f - h + pos.y);
    vertex(0.0f, lh * fz, 0.0f + pos.x, 1.0f + pos.y);
    vertex(w0 * fz, lh * fz, w + pos.x, 1.0f + pos.y);

    index(0, 1, 3, 0, 3, 2);

    _offset.x += w0 * fz;
}

void Text::vertex(float x, float y, float u, float v) {
    const float newVertex[] = {_offset.x + x, _offset.y + y, u, v};
    _vertices.insert(_vertices.end(), std::begin(newVertex), std::end(newVertex));
}

void Text::index(uint a, uint b, uint c, uint d, uint e, uint f) {
    const uint newIndices[] = {
        _indexOffset + a, 
        _indexOffset + b, 
        _indexOffset + c, 
        _indexOffset + d, 
        _indexOffset + e, 
        _indexOffset + f
    };
    _indices.insert(_indices.end(), std::begin(newIndices), std::end(newIndices));
    _indexOffset += 4;
}

Mesh* Text::get_mesh(){
    _mesh = new Mesh(_vertices.data(), _vertices.size(), _indices.data(), _indices.size());
    return _mesh;
}



/*


    class Text : UIelement
    {

    private
        Texture _fontTexture;
    private
        Shader _shader;
    private
        string _text;
    private
        Mesh _mesh;
    private
        Font _font;

    private
        List<float> vertices = new List<float>(1024);
    private
        List<uint> indices = new List<uint>(256);
    private
        uint indexOffset = 0;
        private(float x, float y) offset = (0.0f, 0.0f);

    private
        const int lineHeight = 12;

        // private float [] vertices = [
        //     0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        //     0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        //     0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        //     0.5f, 0.5f, 0.0f, 1.0f, 1.0f
        // ];
        // private uint [] indices = [
        //     0, 1, 3, 0, 3, 2
        // ];

    public
        Text(string text)
        {
            _text = text;
            _fontTexture = Texture.LoadFromFile("res/textures/font.png");
            _shader = new Shader("res/shaders/font.vert", "res/shaders/font.frag");
            _font = new Font("path");
            _mesh = getMesh();
        }

    public
        override void Draw(Camera camera)
        {
            // GL.BlendFuncSeparate(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha, BlendingFactor.One);

            // GL.BlendFunc();
            GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.One);
            _shader.SetMatrix4("transform", Matrix4.CreateTranslation(-0.5f, 0.3f, 0.0f));
            _shader.SetMatrix4("ortho", camera.GetOrthoMatrix());
            _fontTexture.Use(TextureUnit.Texture0);
            _shader.Use();
            _mesh.draw();
            GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha);
        }
    public
        void Update(string newText)
        {
            // _mesh = new Mesh([],[]);
            // _mesh.loadBuffers();
            if (_text == newText)
                return;
            _text = newText;
            _mesh = getMesh();
        }

    public
        Mesh getMesh()
        {

            vertices.Clear();
            indices.Clear();
            indexOffset = 0;
            offset = (0.0f, 0.0f);

            foreach (char c in _text)
            {
                renderCharacter(c);
            }
            return new Mesh(vertices.ToArray(), indices.ToArray());
        }

    public
        void renderCharacter(char character)
        {

            Character ch = _font.getCharacter(character);

            int texWidth = 512;
            int texHeight = 512;
            int lineNum = ch.line;
            int w0 = ch.width;
            int lh = 11;
            int posX = ch.posX;
            int posY = lineNum * lh;
            float fz = 1.0f;

            (float x, float y) pos = ((float)posX / (float)texWidth, (float)posY / (float)texHeight * -1);
            float w = (float)w0 / (float)texWidth;
            float h = (float)lh / (float)texHeight;

            vertex(0.0f, 0.0f, 0.0f, 0.0f + pos.x, 1.0f - h + pos.y);
            vertex(w * fz, 0.0f, 0.0f, w + pos.x, 1.0f - h + pos.y);
            vertex(0.0f, h * fz, 0.0f, 0.0f + pos.x, 1.0f + pos.y);
            vertex(w * fz, h * fz, 0.0f, w + pos.x, 1.0f + pos.y);

            index(0, 1, 3, 0, 3, 2);

            offset.x += w * fz;
        }

    private
        void vertex(float x, float y, float z, float uvx, float uvy)
        {
            // x y z u v r g b
            vertices.AddRange([ offset.x + x, offset.y + y, z, uvx, uvy, 0.0f, 0.0f, 0.0f ]);
        }

    private
        void index(uint a, uint b, uint c, uint d, uint e, uint f)
        {
            indices.Add(indexOffset + a);
            indices.Add(indexOffset + b);
            indices.Add(indexOffset + c);
            indices.Add(indexOffset + d);
            indices.Add(indexOffset + e);
            indices.Add(indexOffset + f);
            indexOffset += 4;
        }

    }

    public class Font
    {

        // private Character [] CharactersData =
    private
        List<Character> CharactersData = [];

    private
        object jsonData;

    private
        Character nullCharacter = new Character(' ', 5, 0, 3);

    public
        Font(string path)
        {

            string jsonString = File.ReadAllText("res/characters.json");
            jsonData = JsonConvert.DeserializeObject(jsonString);

            foreach (PropertyDescriptor descriptor in TypeDescriptor.GetProperties(jsonData))
            {
                char charName = descriptor.Name[0];
                var charProps = (Newtonsoft.Json.Linq.JArray)descriptor.GetValue(jsonData);
                int charWidth = (int)charProps[0];
                int charLine = (int)charProps[1];
                int charPos = (int)charProps[2];
                int charOffsetX = getOffsetX(charLine, charPos);

                CharactersData.Add(new Character(charName, charWidth, charOffsetX, charLine));
            }
        }

    public
        Character getCharacter(char name)
        {
            for (int i = 0; i < CharactersData.Count(); i++)
            {
                if (CharactersData[i].name == name)
                {
                    return CharactersData[i];
                }
            }
            return nullCharacter;
        }

    private
        int getOffsetX(int line, int pos)
        {
            int offsetX = 0;
            foreach (PropertyDescriptor descriptor in TypeDescriptor.GetProperties(jsonData))
            {
                var charProps = (Newtonsoft.Json.Linq.JArray)descriptor.GetValue(jsonData);
                int charWidth = (int)charProps[0];
                int charLine = (int)charProps[1];
                int charPos = (int)charProps[2];
                if (charLine == line && charPos < pos)
                    offsetX += charWidth;
            }
            return offsetX;
        }
    }

    public class Character
    {

    public
        char name;
    public
        int width;
    public
        int line;
    public
        int posX;

    public
        Character(char name, int width, int posX, int line)
        {
            this.name = name;
            this.width = width;
            this.line = line;
            this.posX = posX;
        }
    }

}

*/