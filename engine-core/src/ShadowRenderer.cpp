#include "ShadowRenderer.hpp"

namespace Core
{
    // ----- Public -----

    ShadowRenderer::ShadowRenderer(Shader* shader, uint32 width, uint32 height, glm::vec3 lightPos)
        :   _shader(shader),
            _shadowWidth(width),
            _shadowHeight(height)
    {
        //Configure light space transform
        glm::mat4 orthoProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        _lightProjection = orthoProjection * lightView;

        //Create and configure framebuffer
        _fbo = MakeScope<FrameBuffer>();
        _fbo->CreateDepthTextureAttachment(_shadowWidth, _shadowHeight);
        _fbo->DeleteColorBufferAttachment();
    }

    void ShadowRenderer::StartFrame()
    {
        _fbo->Bind(_shadowWidth, _shadowHeight);
        _shader->Bind();
        _shader->SetUniformMat4f("lightProjection", _lightProjection);
    }

    void ShadowRenderer::EndFrame()
    {
        _fbo->Unbind();
    }

    Shader* ShadowRenderer::GetShader()
    {
        return _shader;
    }

    Texture* ShadowRenderer::GetDepthTexture() const
    {
        return _fbo->GetDepthTexture();
    }
}