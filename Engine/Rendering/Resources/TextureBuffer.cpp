#include "TextureBuffer.hpp"

namespace Engine
{
    // ----- Private -----

    void TextureBuffer::ActivateTextureFlipOnLoad() const
    {
        stbi_set_flip_vertically_on_load(true);
    }

    void TextureBuffer::DeactivateTextureFlipOnLoad() const
    {
        stbi_set_flip_vertically_on_load(false);
    }

    uint32 TextureBuffer::Init(const std::string &filepath)
    {
        uint32 initStatus = EXIT_FAILURE;

        ActivateTextureFlipOnLoad();
        _pxBuffer = stbi_load(filepath.c_str(), (int*)(&_width), (int*)(&_height), (int*)(&_channels), 0);
        DeactivateTextureFlipOnLoad();

        if(_pxBuffer != nullptr)
        {
            if(_saveBackup)
            {
                _backupBuffer = (unsigned char*)malloc(_width * _height * 3);
                if(_backupBuffer != nullptr)
                {
                    std::memcpy(_backupBuffer, _pxBuffer, _width * _height * 3);
                }
                else
                {
                    Logger::Error("Failed", "Texturebuffer", "malloc (" + filepath + ")");
                }
            }

            if(_channels == 1)
            {
                _format = GL_RED;
            }
            else if(_channels == 3)
            {
                _format = GL_RGB;
            }
            else if(_channels == 4)
            {
                _format = GL_RGBA;
            }
            else
            {
                _format = 0;
                Logger::Error("Failed", "Texturebuffer", "Format (" + filepath + ")");
            }

            Logger::Info("Loaded", "Texturebuffer", filepath);
            std::string texInfo = "(X: " + std::to_string(_width)  +
                                  ", Y: " + std::to_string(_height) +
                                  ", Channels: " + std::to_string(_channels) +
                                  ", Format: " + std::to_string(_format) + ")";
            Logger::Info("", "", texInfo);
            initStatus = EXIT_SUCCESS;
        }
        else
        {
            Logger::Error("Failed", "Texturebuffer", "Loading (" + filepath + ")");
        }

        return initStatus;
    }

    // ----- Public -----

    TextureBuffer::TextureBuffer(const std::string &filepath, bool saveBackup)
        :   _initStatus(EXIT_FAILURE), _width(0), _height(0), _channels(0), _format(0),
            _saveBackup(saveBackup), _pxBuffer(nullptr), _backupBuffer(nullptr)
    {
        _initStatus = Init(filepath);
    }

    TextureBuffer::~TextureBuffer()
    {
        stbi_image_free(_pxBuffer);
        if(_saveBackup)
        {
            free(_backupBuffer);
        }
    }

    uint32 TextureBuffer::GetInitStatus() const
    {
        return _initStatus;
    }

    uint32 TextureBuffer::GetWidth() const
    {
        return _width;
    }

    uint32 TextureBuffer::GetHeight() const
    {
        return _height;
    }

    uint32 TextureBuffer::GetChannels() const
    {
        return _channels;
    }

    GLenum TextureBuffer::GetFormat() const
    {
        return _format;
    }

    unsigned char* TextureBuffer::GetRawData() const
    {
        return _pxBuffer;
    }

    bool TextureBuffer::GetPxColor(uint32 x, uint32 y, PxColor* colorOut) const
    {
        if(x < _width && y < _height)
        {
            colorOut->r = *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 0);
            colorOut->g = *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 1);
            colorOut->b = *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 2);
            return true;
        }

        Logger::Error("Failed", "GetPxColor", "Array out of bounds");
        return false;
    }

    bool TextureBuffer::SetPxColor(uint32 x, uint32 y, const PxColor& colorIn) const
    {
        if(x < _width && y < _height)
        {
            *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 0) = colorIn.r;
            *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 1) = colorIn.g;
            *(_pxBuffer + (y * _width * _channels) + (x * _channels) + 2) = colorIn.b;
            return true;
        }

        Logger::Error("Failed", "SetPxColor", "Array out of bounds");
        return false;
    }

    bool TextureBuffer::ResetPxColor(uint32 x, uint32 y) const
    {
        if(_saveBackup)
        {
            const PxColor color = {
                *(_backupBuffer + (y * _width * _channels) + (x * _channels) + 0),
                *(_backupBuffer + (y * _width * _channels) + (x * _channels) + 1),
                *(_backupBuffer + (y * _width * _channels) + (x * _channels) + 2)
            };

            if(SetPxColor(x, y, color))
            {
                return true;
            }
        }
        else
        {
            Logger::Error("Failed", "ResetPxColor", "No backup buffer");
        }

        return false;
    }

    bool TextureBuffer::SubsampleArea(uint32 xpos, uint32 ypos, uint32 sampleAmount, glm::uvec3* colorOut) const
    {
        bool success = true;
        uint32 pxAmount = sampleAmount * sampleAmount;
        PxColor pxColor= {0, 0, 0};

        // For debug purposes:
        if(GetPxColor(xpos, ypos, &pxColor))
        {
            colorOut->x = pxColor.r;
            colorOut->y = pxColor.g;
            colorOut->z = pxColor.b;
            return success;
        }

        for(uint32 x = 0; x < sampleAmount; x++)
        {
            for(uint32 y = 0; y < sampleAmount; y++)
            {
                if(GetPxColor(xpos + x, ypos + y, &pxColor))
                {
                    colorOut->x += pxColor.r;
                    colorOut->y += pxColor.g;
                    colorOut->z += pxColor.b;
                }
                else
                {
                    success = false;
                }
            }
        }

        colorOut->x /= pxAmount;
        colorOut->y /= pxAmount;
        colorOut->z /= pxAmount;

        return success;
    }

    void TextureBuffer::PrintAllValues() const
    {
        for(uint32 x = 0; x < _width; x++)
        {
            for(uint32 y = 0; y < _height; y++)
            {
                PxColor color = {0, 0, 0};
                if(GetPxColor(x, y, &color))
                {
                    Logger::Print("Color (" + std::to_string(x) + ", "
                                            + std::to_string(y) + ") : "
                                            + std::to_string(color.r) + ", "
                                            + std::to_string(color.g) + ", "
                                            + std::to_string(color.b));
                }
            }
        }
    }
}  // namespace Engine
