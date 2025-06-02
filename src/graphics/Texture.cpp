#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

Texture::Texture(std::string texturePath){
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // loading texture from file
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        throw std::runtime_error("ERROR! Cannot load image data from " + texturePath);
    }
    stbi_image_free(data);
}

GLuint Texture::getID(){
    return this->texID;
}