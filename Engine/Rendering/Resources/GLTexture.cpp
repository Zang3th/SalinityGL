#include "GLTexture.hpp"

namespace Engine
{
    // ----- Private -----

    uint32 GLTexture::Init()
    {
        uint32 initStatus = EXIT_FAILURE;

        if(_textureBuffer->GetInitStatus() == EXIT_SUCCESS)
        {
            //Save internal parameters
            _width    = _textureBuffer->GetWidth();
            _height   = _textureBuffer->GetHeight();
            _channels = _textureBuffer->GetChannels();
            _format   = _textureBuffer->GetFormat();

            //Create texture from buffer
            GLCall(glGenTextures(1, &_textureID))
            Bind();
            GLCall(glTexImage2D(GL_TEXTURE_2D,
                                0,
                                _format,
                                _width,
                                _height,
                                0,
                                _format,
                                GL_UNSIGNED_BYTE,
                                _textureBuffer->GetRawData()))
            Unbind();

            Logger::Info("Created", "GLTexture", _name + " (Texture-ID: " + std::to_string(_textureID) + ")");
            initStatus = EXIT_SUCCESS;
        }
        else
        {
            Logger::Error("Failed", "GLTexture", _name + " (Texture-ID: " + std::to_string(_textureID) + ")");
        }

        return initStatus;
    }

    uint32 GLTexture::Create(GLint internalFormat, GLenum type)
    {
        GLCall(glGenTextures(1, &_textureID))
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, _format, type, nullptr))
        Unbind();

        std::string texInfo = " (X: " + std::to_string(_width)  +
                              ", Y: " + std::to_string(_height) +
                              ", Channels: " + std::to_string(_channels) + ")";
        Logger::Info("Created", "GLTexture", _name + texInfo);

        return EXIT_SUCCESS;
    }

    // ----- Public -----

    GLTexture::GLTexture(std::string name, const TextureBuffer* texBuffer)
        :   _name(std::move(name)), _initStatus(EXIT_FAILURE), _width(0), _height(0), _channels(0), _textureID(0),
            _numberOfRows(0), _format(0), _ownsTexBufPointer(false), _textureBuffer(texBuffer)
    {
        _initStatus = Init();
    }

    GLTexture::GLTexture(std::string name, const std::string& filepath, bool saveBackup, uint32 numberOfRows)
        :   _name(std::move(name)), _initStatus(EXIT_FAILURE), _width(0), _height(0), _channels(0), _textureID(0),
            _numberOfRows(numberOfRows), _format(0), _ownsTexBufPointer(true), _textureBuffer(nullptr)
    {
        _textureBuffer = new TextureBuffer(filepath, saveBackup);
        _initStatus = Init();
    }

    GLTexture::GLTexture(std::string name, uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type)
        :   _name(std::move(name)), _initStatus(EXIT_FAILURE), _width(width), _height(height), _channels(0), _textureID(0),
            _numberOfRows(0), _format(format), _ownsTexBufPointer(false), _textureBuffer(nullptr)
    {
        _initStatus = Create(internalFormat, type);
    }

    GLTexture::~GLTexture()
    {
        GLCall(glDeleteTextures(1, &_textureID))

        if(_ownsTexBufPointer == true)
        {
            delete _textureBuffer;
        }
    }

    void GLTexture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0))
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID))
    }

    void GLTexture::BindToSlot(const uint32 slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot))
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID))
    }

    void GLTexture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0))
    }

    void GLTexture::UploadModifiedBuffer() const
    {
        Bind();
        GLCall(glTexSubImage2D(GL_TEXTURE_2D,
                               0,
                               0,
                               0,
                               _width,
                               _height,
                               _format,
                               GL_UNSIGNED_BYTE,
                               _textureBuffer->GetRawData()));
        Unbind();
    }

    void GLTexture::AddMipmapLinear() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
        GLCall(glGenerateMipmap(GL_TEXTURE_2D))

        Logger::Info("Added", "Mipmap", "Texture-ID: " + std::to_string(_textureID));
    }

    void GLTexture::AddAnisotropicFilter(const float strength) const
    {
        GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, strength))
    }

    void GLTexture::AddMinFilterNearest() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST))
    }

    void GLTexture::AddMaxFilterNearest() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
    }

    void GLTexture::AddMinFilterLinear() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
    }

    void GLTexture::AddMaxFilterLinear() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    }

    void GLTexture::AddWrapRepeat() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
    }

    void GLTexture::AddClampToEdge() const
    {
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
    }

    void GLTexture::AddBorderColor(const glm::vec4& color) const
    {
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color[0]))
    }

    uint32 GLTexture::GetWidth() const
    {
        return _width;
    }

    uint32 GLTexture::GetHeight() const
    {
        return _height;
    }

    uint32 GLTexture::GetTextureID() const
    {
        return _textureID;
    }

    uint32 GLTexture::GetNumberOfRows() const
    {
        return _numberOfRows;
    }

    const TextureBuffer* GLTexture::GetTextureBuffer() const
    {
        return _textureBuffer;
    }
}
