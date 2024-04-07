#pragma once

#include "glad/gl.h"

namespace Blib {

class Texture {
public:
    // holds the ID of the texture object, used for all texture operations to
    // reference to this particular texture
    unsigned int ID = 0;
    // texture image dimensions
    unsigned int Width = 0, Height = 0; // width and height of loaded image in pixels
    // texture Format
    unsigned int Internal_Format = GL_RGB; // format of texture object
    unsigned int Image_Format = GL_RGB;    // format of loaded image
    // texture configuration
    unsigned int Wrap_S = GL_REPEAT;     // wrapping mode on S axis
    unsigned int Wrap_T = GL_REPEAT;     // wrapping mode on T axis
    unsigned int Filter_Min = GL_LINEAR; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max = GL_LINEAR; // filtering mode if texture pixels > screen pixels

    Texture() { glGenTextures(1, &this->ID); }
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char *data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const { glBindTexture(GL_TEXTURE_2D, this->ID); }
};

} // namespace Blib
