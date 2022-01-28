#include "WaterRenderer.hpp"

namespace Core
{
    // ----- Private -----

    void WaterRenderer::InitReflectionFBO()
    {
        _reflectFBO = MakeScope<FrameBuffer>();
        _reflectFBO->Bind();
        _reflectFBO->SetColorAttachment0();
        _reflectFBO->CreateTextureAttachment(_reflectionWidth, _reflectionHeight);
        _reflectFBO->CreateDepthBufferAttachment(_reflectionWidth, _reflectionHeight);
        _reflectFBO->Unbind();
    }

    void WaterRenderer::InitRefractionFBO()
    {
        _refractFBO = MakeScope<FrameBuffer>();
        _refractFBO->Bind();
        _refractFBO->SetColorAttachment0();
        _refractFBO->CreateTextureAttachment(_refractionWidth, _refractionHeight);
        _refractFBO->CreateDepthTextureAttachment(_refractionWidth, _refractionHeight);
        _refractFBO->Unbind();
    }

    void WaterRenderer::StartReflectionFrame()
    {
        _reflectFBO->Bind(_reflectionWidth, _reflectionHeight);
    }

    void WaterRenderer::EndReflectionFrame()
    {
        _reflectFBO->Unbind();
    }

    void WaterRenderer::StartRefractionFrame()
    {
        _refractFBO->Bind(_refractionWidth, _refractionHeight);
    }

    void WaterRenderer::EndRefractionFrame()
    {
        _refractFBO->Unbind();
    }

    void WaterRenderer::RenderReflectionFrame(const glm::mat4 lightProjection)
    {
        //Render to reflection framebuffer
        StartReflectionFrame();

        Renderer::ClearBuffers();
        _waterShader->Bind();
        _waterShader->SetUniformVec4f("clipPlane", _reflectionClipPlane);
        _waterShader->Unbind();
        Renderer::FlushAllModels(_waterShader, lightProjection);
        Renderer::FlushCubemap();

        EndReflectionFrame();
    }

    void WaterRenderer::RenderRefractionFrame(const glm::mat4 lightProjection)
    {
        //Render to refraction framebuffer
        StartRefractionFrame();

        Renderer::ClearBuffers();
        _waterShader->Bind();
        _waterShader->SetUniformVec4f("clipPlane", _refractionClipPlane);
        _waterShader->Unbind();
        Renderer::FlushAllModels(_waterShader, lightProjection);
        Renderer::FlushCubemap();

        EndRefractionFrame();
    }

    // ----- Public -----

    WaterRenderer::WaterRenderer(Shader* waterShader)
        : _waterShader(waterShader)
    {
        InitReflectionFBO();
        InitRefractionFBO();
    }

    void WaterRenderer::Render(const glm::mat4 lightProjection)
    {
        GLCall(glEnable(GL_CLIP_DISTANCE0));
        RenderReflectionFrame(lightProjection);
        RenderRefractionFrame(lightProjection);
        GLCall(glDisable(GL_CLIP_DISTANCE0));
    }

    Texture* WaterRenderer::GetReflectTexture() const
    {
        return _reflectFBO->GetTexture();
    }

    Texture* WaterRenderer::GetRefractTexture() const
    {
        return _refractFBO->GetTexture();
    }

    Texture* WaterRenderer::GetRefractDepthTexture() const
    {
        return _refractFBO->GetDepthTexture();
    }
}