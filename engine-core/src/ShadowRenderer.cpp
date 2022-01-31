#include "ShadowRenderer.hpp"

namespace Core
{
    // ----- Private -----

    void ShadowRenderer::StartFrame()
    {
        GLCall(glCullFace(GL_FRONT));
        _fbo->Bind(_shadowWidth, _shadowHeight);
    }

    void ShadowRenderer::EndFrame()
    {
        _fbo->Unbind();
        GLCall(glCullFace(GL_BACK));
    }

    // ----- Public -----

    ShadowRenderer::ShadowRenderer(uint32 width, uint32 height, glm::vec3 lightPos)
        :   _orthoProjection(glm::ortho(-90.0f, 90.0f, -90.0f, 90.0f, 115.0f, 200.0f)),
            _lightView(glm::lookAt(lightPos, glm::vec3(64.0f, 0.0f, 64.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
            _lightProjection(_orthoProjection * _lightView),
            _shadowWidth(width),
            _shadowHeight(height)
    {
        //Create and configure framebuffer
        _fbo = MakeScope<FrameBuffer>();
        _fbo->Bind();
        _fbo->CreateDepthTextureAttachment("ShadowDepthTexture", _shadowWidth, _shadowHeight);
        _fbo->DeleteColorBufferAttachment();
        _fbo->Unbind();
    }

    void ShadowRenderer::RenderToFramebuffer(Shader* shadowShader)
    {
        //Render scene to shadow framebuffer
        StartFrame();
        Renderer::ClearBuffers();
        Renderer::FlushShadowModels(shadowShader, _lightProjection);
        EndFrame();
    }

    Texture* ShadowRenderer::GetDepthTexture() const
    {
        return _fbo->GetDepthTexture();
    }

    glm::mat4 ShadowRenderer::GetLightProjection() const
    {
        return _lightProjection;
    }
}