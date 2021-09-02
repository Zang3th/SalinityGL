#include "ShadowRenderer.hpp"

namespace Core
{
    // ----- Public -----

    ShadowRenderer::ShadowRenderer(Shader* shader, uint32 width, uint32 height, glm::vec3 lightPos)
    :   _orthoProjection(glm::ortho(-90.0f, 90.0f, -90.0f, 90.0f, 1.0f, 300.0f)),
            _lightView(glm::lookAt(lightPos, glm::vec3(64.0f, 0.0f, 64.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
            _lightProjection(_orthoProjection * _lightView),
            _shader(shader),
            _shadowWidth(width),
            _shadowHeight(height)
    {
        //Create and configure framebuffer
        _fbo = MakeScope<FrameBuffer>();
        _fbo->CreateDepthTextureAttachment(_shadowWidth, _shadowHeight);
        _fbo->DeleteColorBufferAttachment();
    }

    void ShadowRenderer::StartFrame()
    {
        GLCall(glCullFace(GL_FRONT));
        _fbo->Bind(_shadowWidth, _shadowHeight);
        _shader->Bind();
        _shader->SetUniformMat4f("lightProjection", _lightProjection);
    }

    void ShadowRenderer::EndFrame()
    {
        _fbo->Unbind();
        GLCall(glCullFace(GL_BACK));
    }

    Shader* ShadowRenderer::GetShader()
    {
        return _shader;
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