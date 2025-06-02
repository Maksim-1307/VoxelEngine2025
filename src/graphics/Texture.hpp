#pragma once

#include <iostream>
#include <GL/glew.h>

class Texture
{

public:
    Texture(std::string texturePath);
    Texture(){};
    ~Texture();

    GLuint getID();

private:
    unsigned int texID = 0;
};
