#include "Texture.hpp"

namespace Engine
{
    // ----- Private -----

    void Texture::InitFromFile(const std::string &filepath)
    {
        ActivateTextureFlipOnLoad();
        _imgBuffer = stbi_load(filepath.c_str(), &_width, &_height, &_nrChannels, 0);
        DeactivateTextureFlipOnLoad();

        if(_imgBuffer != nullptr)
        {
            if(_saveToBuffer)
            {
                _backupBuffer = (unsigned char*)malloc(_width * _height * 3);
                if(_backupBuffer != nullptr)
                {
                    std::memcpy(_backupBuffer, _imgBuffer, _width * _height * 3);
                }
            }

            if(_nrChannels == 1)
            {
                _format = GL_RED;
            }
            else if(_nrChannels == 3)
            {
                _format = GL_RGB;
            }
            else if(_nrChannels == 4)
            {
                _format = GL_RGBA;
            }
            else
            {
                _format = 0;
                Logger::Error("Failed", "Image-Texture-Format", filepath);
            }

            if(_format != 0)
            {
                //Create texture
                GLCall(glGenTextures(1, &_textureID))
                Bind();
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _imgBuffer))

                //Texture parameters
                GLCall(glGenerateMipmap(GL_TEXTURE_2D))
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))

                //Activate anisotropic filtering
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0))
                GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f))
                Unbind();

                std::string texInfo = "(X: " + std::to_string(_width) + ", Y: " + std::to_string(_height) + ", Channels: " + std::to_string(_nrChannels) + ")";
                Logger::Info("Loaded", "Texture", filepath);
                Logger::Info("", "", texInfo);
            }
        }
        else
        {
            Logger::Error("Failed", "Texture-Load", filepath);
        }

        if(!_saveToBuffer)
        {
            stbi_image_free(_imgBuffer);
        }
    }

    void Texture::Create(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type)
    {
        //Create texture
        GLCall(glGenTextures(1, &_textureID))
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr))

        std::string texInfo = "(X: " + std::to_string(width) + ", Y: " + std::to_string(height) + ", Format: " + std::to_string(format) + ")";
        Logger::Info("Created", "Texture", texInfo);
    }

    //Warning: Raw/unsafe accessing of the pixel buffer.
    //Be sure to check bounds and saved status before!
    glm::uvec3 Texture::GetPxColorRaw(uint32 x, uint32 y) const
    {
        glm::uvec3 colorOut;
        colorOut.x = *(_imgBuffer + (y * _width * 3) + (x * 3) + 0);
        colorOut.y = *(_imgBuffer + (y * _width * 3) + (x * 3) + 1);
        colorOut.z = *(_imgBuffer + (y * _width * 3) + (x * 3) + 2);
        return colorOut;
    }

    // ----- Public -----

    Texture::Texture(const std::string &filepath, uint32 numberOfRows, bool saveToBuffer)
        :   _width(0), _height(0), _nrChannels(0), _format(0), _textureID(0), _numberOfRows(numberOfRows), _saveToBuffer(saveToBuffer), _imgBuffer(nullptr), _backupBuffer(nullptr)
    {
        InitFromFile(filepath);
    }

    Texture::Texture(int32 width, int32 height, GLint internalFormat, GLenum format, GLenum type)
        :   _width(width), _height(height), _nrChannels(0), _format(format), _textureID(0), _numberOfRows(0), _saveToBuffer(false), _imgBuffer(nullptr), _backupBuffer(nullptr)
    {
        Create(width, height, internalFormat, format, type);
    }

    Texture::~Texture()
    {
        GLCall(glDeleteTextures(1, &_textureID))

        if(_saveToBuffer)
        {
            stbi_image_free(_imgBuffer);
            free(_backupBuffer);
        }
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

    void Texture::ModifyTexture(uint32 x, uint32 y, const glm::vec3& color)
    {
        if(_saveToBuffer)
        {
            if((int32)x < _width && (int32)y < _height)
            {
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 0) = (unsigned char)(color.x * 255);
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 1) = (unsigned char)(color.y * 255);
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 2) = (unsigned char)(color.z * 255);
            }
            {
                Logger::Error("Failed", "Modification", "Array access out of bounds");
            }
        }
        else
        {
            Logger::Error("Failed", "Modification", "Texture wasn't saved");
        }
    }

    void Texture::ResetTextureModification(uint32 x, uint32 y)
    {
        if(_saveToBuffer)
        {
            if((int32)x < _width && (int32)y < _height)
            {
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 0) = *(_backupBuffer + (y * _width * 3) + (x * 3) + 0);
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 1) = *(_backupBuffer + (y * _width * 3) + (x * 3) + 1);
                *(_imgBuffer + (y * _width * 3) + (x * 3) + 2) = *(_backupBuffer + (y * _width * 3) + (x * 3) + 2);
            }
            {
                Logger::Error("Failed", "Modification", "Array access out of bounds");
            }
        }
        else
        {
            Logger::Error("Failed", "Modification", "Texture wasn't saved");
        }
    }

    void Texture::CommitModifications() const
    {
        Bind();
        GLCall(glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, _imgBuffer));
        Unbind();
    }

    uint32 Texture::GetWidth() const
    {
        return (uint32)_width;
    }

    uint32 Texture::GetHeight() const
    {
        return (uint32)_height;
    }

    uint32 Texture::GetTextureID() const
    {
        return _textureID;
    }

    uint32 Texture::GetNumberOfRows() const
    {
        return _numberOfRows;
    }

    bool Texture::Subsample(uint32 xpos, uint32 ypos, uint32 sampleAmount, glm::uvec3* colorOut) const
    {
        if(_saveToBuffer == false)
        {
            Logger::Error("Failed", "Subsampling", "Texture wasn't saved");
            return false;
        }

        *colorOut = GetPxColorRaw(xpos, ypos);
        return true;

        uint32 pxAmount = sampleAmount * sampleAmount;
        glm::uvec3 totalColor = {0, 0, 0};

        for(uint32 x = 0; x < sampleAmount; x++)
        {
            for(uint32 y = 0; y < sampleAmount; y++)
            {
                if((int32)(xpos + x) < _width && (int32)(ypos + y)< _height)
                {
                    totalColor += GetPxColorRaw(xpos + x, ypos + y);
                }
                else
                {
                    Logger::Error("Failed", "Subsampling", "Array access out of bounds");
                }
            }
        }

        colorOut->x = totalColor.x / pxAmount;
        colorOut->y = totalColor.y / pxAmount;
        colorOut->z = totalColor.z / pxAmount;

        return true;
    }
}
