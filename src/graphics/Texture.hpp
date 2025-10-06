#pragma once

#include <iostream>
#include <GL/glew.h>

class Texture
{

public:
    Texture(std::string texturePath, bool alpha=false);
    Texture(unsigned char* data, int width, int height);
    Texture(int width, int height);
    Texture(){};
    ~Texture(){
        glDeleteTextures(1, &texID);
    };

    GLuint getID();

private:
    unsigned int texID = 0;
};
