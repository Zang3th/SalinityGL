#include "FrameBuffer.hpp"

namespace Core
{
    // ----- Public -----

    FrameBuffer::FrameBuffer()
        :   _fboID(0),
            _textureID(nullptr),
            _depthTextureID(nullptr),
            _renderBufferID(nullptr)
    {
        GLCall(glGenFramebuffers(1, &_fboID));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _fboID));
        GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
    }

    FrameBuffer::~FrameBuffer()
    {
        GLCall(glDeleteFramebuffers(1, &_fboID));

        if(_textureID){
            GLCall(glDeleteTextures(1, _textureID.get()));}

        if(_renderBufferID){
            GLCall(glDeleteRenderbuffers(1, _textureID.get()));}

        if(_depthTextureID){
            GLCall(glDeleteTextures(1, _textureID.get()));}
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

    void FrameBuffer::CreateTextureAttachment(const uint32 width, const uint32 height)
    {
        _textureID = MakeScope<uint32>();
        GLCall(glGenTextures(1, _textureID.get()));
        GLCall(glBindTexture(GL_TEXTURE_2D, *(_textureID.get())));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *(_textureID.get()), 0));
    }

    void FrameBuffer::CreateDepthTextureAttachment(const uint32 width, const uint32 height)
    {
        _depthTextureID = MakeScope<uint32>();
        GLCall(glGenTextures(1, _depthTextureID.get()));
        GLCall(glBindTexture(GL_TEXTURE_2D, *(_depthTextureID.get())));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *(_depthTextureID.get()), 0));
    }

    void FrameBuffer::CreateRenderBufferAttachment(const uint32 width, const uint32 height)
    {
        _renderBufferID = MakeScope<uint32>();
        GLCall(glGenRenderbuffers(1, _renderBufferID.get()));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, *(_renderBufferID.get())));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *(_renderBufferID.get())));
    }
}