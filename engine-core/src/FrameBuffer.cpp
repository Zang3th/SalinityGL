#include "FrameBuffer.hpp"

namespace Core
{
    // ----- Public -----

    FrameBuffer::FrameBuffer()
        :   _fboID(0),
            _depthBufferID(0),
            _texture(nullptr),
            _depthTexture(nullptr)
    {
        GLCall(glGenFramebuffers(1, &_fboID));
    }

    FrameBuffer::~FrameBuffer()
    {
        GLCall(glDeleteFramebuffers(1, &_fboID));
    }

    void FrameBuffer::Bind() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
    }

    void FrameBuffer::Bind(const uint32 width, const uint32 height) const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
        GLCall(glViewport(0, 0, width, height));
    }

    void FrameBuffer::Unbind() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GLCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    }

    void FrameBuffer::SetColorAttachment0()
    {
        GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
    }

    Texture* FrameBuffer::CreateTextureAttachment(const std::string& name, uint32 width, uint32 height)
    {
        //Create and configure texture
        _texture = ResourceManager::LoadTexture(name, width, height, GL_RGB);

        //Attach texture
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->GetTextureID(), 0));

        return _texture;
    }

    Texture* FrameBuffer::CreateDepthTextureAttachment(const std::string& name, const uint32 width, const uint32 height)
    {
        //Create and configure depth texture
        _depthTexture = ResourceManager::LoadTexture(name, width, height, GL_DEPTH_COMPONENT);

        //Attach depth texture
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture->GetTextureID(), 0));

        return _depthTexture;
    }

    void FrameBuffer::CreateDepthBufferAttachment(uint32 width, uint32 height)
    {
        GLCall(glGenRenderbuffers(1, &_depthBufferID));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferID));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferID));
    }

    void FrameBuffer::DeleteColorBufferAttachment() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    Texture* FrameBuffer::GetTexture() const
    {
        return _texture;
    }

    Texture* FrameBuffer::GetDepthTexture() const
    {
        return _depthTexture;
    }
}