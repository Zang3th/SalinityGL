#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"

//Anisotropic filtering is implemented as an extension and isnt included in OpenGL Core
//So I need to define the value myself or include the extension
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

class Texture
{
    private:
        unsigned int _textureID;

    public:
        Texture(const std::string& filepath);
        ~Texture();
        void Bind();
        void BindToSlot(unsigned int slot);
        void Unbind();
};