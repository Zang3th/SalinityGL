#include "FrameBuffer.hpp"

namespace Core
{
    // ----- Public -----

    FrameBuffer::FrameBuffer()
        :   _fboID(0),
            _depthTexture(nullptr)
    {
        GLCall(glGenFramebuffers(1, &_fboID));
    }

    FrameBuffer::~FrameBuffer()
    {
        GLCall(glDeleteFramebuffers(1, &_fboID));
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

    void FrameBuffer::DeleteColorBufferAttachment() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void FrameBuffer::CreateDepthTextureAttachment(const uint32 width, const uint32 height)
    {
        //Create texture name
        std::string textureName = "DepthTexture_" + std::to_string(_fboID);

        //Create depth texture
        ResourceManager::LoadDepthTexture(textureName, width, height);
        _depthTexture = ResourceManager::GetTexture(textureName);

        //Attach depth texture
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture->GetTextureID(), 0));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    Texture* FrameBuffer::GetDepthTexture() const
    {
        return _depthTexture;
    }
}