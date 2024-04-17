#include "Texture.hpp"

namespace Engine
{
    // ----- Private -----

    void Texture::InitFromFile(const std::string &filepath)
    {
        int32 width, height, nrChannels;

        ActivateTextureFlipOnLoad();
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        DeactivateTextureFlipOnLoad();

        if(localBuffer)
        {
            GLenum format = 0;

            if(nrChannels == 1)
                format = GL_RED;
            else if(nrChannels == 3)
                format = GL_RGB;
            else if(nrChannels == 4)
                format = GL_RGBA;
            else
                Logger::Error("Failed", "Image-Texture-Format", filepath);

            if(format != 0)
            {
                GLCall(glGenTextures(1, &_textureID))
                Bind();

                //Texture parameters
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer))
                GLCall(glGenerateMipmap(GL_TEXTURE_2D))
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))

                //Activate anisotropic filtering
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0))
                GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f))

                std::string texInfo = "(X: " + std::to_string(width) + ", Y: " + std::to_string(height) + ", Channels: " + std::to_string(nrChannels) + ")";
                Logger::Info("Loaded", "Texture", filepath);
                Logger::Info("", "", texInfo);
            }
        }
        else
            Logger::Error("Failed", "Texture-Load", filepath);

        stbi_image_free(localBuffer);
    }

    void Texture::Init(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type)
    {
        //Create texture
        GLCall(glGenTextures(1, &_textureID))
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr))

        std::string texInfo = "(X: " + std::to_string(width) + ", Y: " + std::to_string(height) + ", Format: " + std::to_string(format) + ")";
        Logger::Info("Created", "Texture", texInfo);
    }

    // ----- Public -----

    Texture::Texture(const std::string &filepath)
        :   _textureID(0), _numberOfRows(0)
    {
        InitFromFile(filepath);
    }

    Texture::Texture(const std::string &filepath, uint32 numberOfRows)
            :   _textureID(0), _numberOfRows(numberOfRows)
    {
        InitFromFile(filepath);
    }

    Texture::Texture(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type)
        :   _textureID(0), _numberOfRows(0)
    {
        Init(width, height, internalFormat, format, type);
    }

    Texture::~Texture()
    {
        GLCall(glDeleteTextures(1, &_textureID))
    }

    void Texture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0))
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID))
    }

    void Texture::BindToSlot(const uint32 slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot))
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID))
    }

    void Texture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0))
    }

    void Texture::ActivateTextureFlipOnLoad() const
    {
        stbi_set_flip_vertically_on_load(true);
    }

    void Texture::DeactivateTextureFlipOnLoad() const
    {
        stbi_set_flip_vertically_on_load(false);
    }

    void Texture::AddFilterNearest() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
    }

    void Texture::AddFilterLinear() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    }

    void Texture::AddWrapRepeat() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
    }

    void Texture::ClampToEdge() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
    }

    void Texture::AddBorderColor() const
    {
        Bind();
        const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor))
    }

    uint32 Texture::GetTextureID() const
    {
        return _textureID;
    }

    uint32 Texture::GetNumberOfRows() const
    {
        return _numberOfRows;
    }
}