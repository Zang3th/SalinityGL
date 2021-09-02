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

    void Texture::InitWithAttribute(const uint32 width, const uint32 height, const Texture_Attribute attribute)
    {
        //Resolve attribute
        GLenum component = 0;

        if(attribute == DEPTH)
            component = GL_DEPTH_COMPONENT;

        //Create texture
        GLCall(glGenTextures(1, &_textureID));
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, component, GL_FLOAT, nullptr));

        //Configure texture
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

        LOG(INFO) << "Created:  Texture | (X: " << width << " | Y: " << height << " | Type: " << std::to_string(component) << ")";
    }

    // ----- Public -----

    Texture::Texture(const std::string &filepath)
        :   _textureID(0)
    {
        InitFromFile(filepath);
    }

    Texture::Texture(const uint32 width, const uint32 height, const Texture_Attribute attribute)
        :   _textureID(0)
    {
        InitWithAttribute(width, height, attribute);
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

    uint32 Texture::GetTextureID() const
    {
        return _textureID;
    }

    void Texture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}