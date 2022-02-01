#include "WaterRenderer.hpp"

namespace Core
{
    // ----- Private -----

    void WaterRenderer::InitReflectionFBO()
    {
        //Create and configure framebuffer
        _reflectFBO = MakeScope<FrameBuffer>();
        _reflectFBO->Bind();
        _reflectFBO->SetColorAttachment0();
        _reflectFBO->CreateDepthBufferAttachment(_reflectionWidth, _reflectionHeight);

        //Create and configure texture attachment
        Texture* reflectTexture = _reflectFBO->CreateTextureAttachment("ReflectionTexture", _reflectionWidth, _reflectionHeight);
        reflectTexture->AddFilterLinear();

        _reflectFBO->Unbind();
    }

    void WaterRenderer::InitRefractionFBO()
    {
        //Create and configure framebuffer
        _refractFBO = MakeScope<FrameBuffer>();
        _refractFBO->Bind();
        _refractFBO->SetColorAttachment0();

        //Create and configure texture attachments
        Texture* refractTexture =_refractFBO->CreateTextureAttachment("RefractionTexture", _refractionWidth, _refractionHeight);
        refractTexture->AddFilterLinear();
        Texture* refractDepthTexture = _refractFBO->CreateDepthTextureAttachment("RefractionDepthTexture", _refractionWidth, _refractionHeight);
        refractDepthTexture->AddFilterLinear();

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

    void WaterRenderer::RenderReflectionFrame(Shader* modelShader)
    {
        //Render to reflection framebuffer
        StartReflectionFrame();

        Renderer::ClearBuffers();
        modelShader->Bind();
        modelShader->SetUniformVec4f("clipPlane", _reflectionClipPlane);
        modelShader->Unbind();
        Renderer::FlushAllModelBuffers(modelShader);
        Renderer::FlushCubemap();

        EndReflectionFrame();
    }

    void WaterRenderer::RenderRefractionFrame(Shader* terrainShader, Shader* modelShader)
    {
        //Render to refraction framebuffer
        StartRefractionFrame();

        Renderer::ClearBuffers();
        terrainShader->Bind();
        terrainShader->SetUniformVec4f("clipPlane", _refractionClipPlane);
        terrainShader->Unbind();
        modelShader->Bind();
        modelShader->SetUniformVec4f("clipPlane", _refractionClipPlane);
        modelShader->Unbind();
        Renderer::FlushTerrainModel(terrainShader);
        Renderer::FlushAllModelBuffers(modelShader);

        EndRefractionFrame();
    }

    // ----- Public -----

    WaterRenderer::WaterRenderer()
    {
        InitReflectionFBO();
        InitRefractionFBO();
    }

    void WaterRenderer::RenderToFramebuffer(Shader* terrainShader, Shader* modelShader)
    {
        GLCall(glEnable(GL_CLIP_DISTANCE0));
        RenderReflectionFrame(modelShader);
        RenderRefractionFrame(terrainShader, modelShader);
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