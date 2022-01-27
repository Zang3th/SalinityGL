#include "Texture.hpp"

namespace Core
{
    // ----- Private -----

    void Texture::InitFromFile(const std::string &filepath)
    {
        int32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

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
                LOG(ERROR) << "Failed:   Imageformat is not supported | " << filepath;

            if(format != 0)
            {
                GLCall(glGenTextures(1, &_textureID));
                Bind();

                //Texture parameters
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer));
                GLCall(glGenerateMipmap(GL_TEXTURE_2D));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

                //Activate anisotropic filtering
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0));
                GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f));

                LOG(INFO) << "Loaded:   Texture | " << filepath << " (X: " << width << " | Y: " << height << " | Channels: " << nrChannels << ")";
            }
        }
        else
        {
            LOG(ERROR) << "Failed:   Texture-Loading | " << filepath;
        }

        stbi_image_free(localBuffer);
    }

    void Texture::Init(uint32 width, uint32 height, GLenum format)
    {
        //Create texture
        GLCall(glGenTextures(1, &_textureID));
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));

        LOG(INFO) << "Created:  Texture | (X: " << width << " | Y: " << height << " | Format: " << std::to_string(format) << ")";
    }

    // ----- Public -----

    Texture::Texture(const std::string &filepath)
        :   _textureID(0)
    {
        InitFromFile(filepath);
    }

    Texture::Texture(const uint32 width, const uint32 height, const GLenum format)
        :   _textureID(0)
    {
        Init(width, height, format);
    }

    Texture::~Texture()
    {
        GLCall(glDeleteTextures(1, &_textureID));
    }

    void Texture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    void Texture::BindToSlot(const uint32 slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    void Texture::AddFilterNearest() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

    void Texture::AddFilterLinear() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    void Texture::AddWrapRepeat() const
    {
        Bind();
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }

    void Texture::AddBorderColor() const
    {
        Bind();
        const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
    }

    uint32 Texture::GetTextureID() const
    {
        return _textureID;
    }

    void Texture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}