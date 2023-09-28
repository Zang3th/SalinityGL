#include "ShadowRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    ShadowRenderer::ShadowRenderer(const uint32 width, const uint32 height, const glm::vec3& lightPos,
                                   const glm::vec3& targetPos, const glm::mat4& orthoProj, Shader* shader)
        :   _lightView(glm::lookAt(lightPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f))),
            _lightProjection(orthoProj * _lightView),
            _shadowWidth(width), _shadowHeight(height), _shadowShader(shader)
    {
        //Create and configure framebuffer
        _fbo = MakeScope<FrameBuffer>();
        _fbo->DeleteColorBufferAttachment();
        _fbo->Bind();

        //Create and configure depth texture attachment
        Texture* depthTexture = _fbo->CreateDepthTextureAttachment("ShadowDepthTexture", _shadowWidth, _shadowHeight);
        depthTexture->AddFilterNearest();
        depthTexture->ClampToEdge();
        depthTexture->AddBorderColor();

        _fbo->Unbind();

        Logger::Info("Created", "Renderer",__func__);
    }

    void ShadowRenderer::StartFrame()
    {
        _fbo->Bind(_shadowWidth, _shadowHeight);
    }

    void ShadowRenderer::EndFrame()
    {
        _fbo->Unbind();
    }

    // ----- Public -----

    void ShadowRenderer::Flush(Renderer* sceneRenderer)
    {
        //Render scene to shadow framebuffer
        StartFrame();
        GLRenderSettings::ClearBuffers();
        ((SceneRenderer*)sceneRenderer)->FlushModels(_shadowShader);
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