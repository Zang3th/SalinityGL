#include "WaterRenderer.hpp"

namespace Engine
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
        GLTexture* reflectTexture = _reflectFBO->CreateTextureAttachment("ReflectionTexture", _reflectionWidth, _reflectionHeight);
        reflectTexture->AddMinFilterLinear();
        reflectTexture->AddMaxFilterLinear();

        _reflectFBO->Unbind();
    }

    void WaterRenderer::InitRefractionFBO()
    {
        //Create and configure framebuffer
        _refractFBO = MakeScope<FrameBuffer>();
        _refractFBO->Bind();
        _refractFBO->SetColorAttachment0();

        //Create and configure texture attachments
        GLTexture* refractTexture = _refractFBO->CreateTextureAttachment("RefractionTexture", _refractionWidth, _refractionHeight);
        refractTexture->Bind();
        refractTexture->AddMinFilterLinear();
        refractTexture->AddMaxFilterLinear();
        refractTexture->Unbind();

        GLTexture* refractDepthTexture = _refractFBO->CreateDepthTextureAttachment("RefractionDepthTexture", _refractionWidth, _refractionHeight);
        refractDepthTexture->Bind();
        refractDepthTexture->AddMinFilterLinear();
        refractDepthTexture->AddMaxFilterLinear();
        refractDepthTexture->Unbind();

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

    void WaterRenderer::RenderReflectionFrame(SceneRenderer* sceneRenderer)
    {
        //Render to reflection framebuffer
        StartReflectionFrame();
        GLRenderSettings::ClearBuffers();

        //Move camera under the water
        glm::vec3 camPos = Camera3D::GetPosition();
        float distance = 2.0f * (camPos.y + _waterHeight);
        Camera3D::SetPosition(glm::vec3(camPos.x, camPos.y - distance, camPos.z));
        Camera3D::InvertPitch();
        Camera3D::Update();

        //Set shader variable(s)
        Shader* modelShader = sceneRenderer->GetModelShader();
        modelShader->Bind();
        modelShader->SetUniformVec4f("clipPlane", _reflectionClipPlane);
        modelShader->Unbind();

        //Render scene
        sceneRenderer->FlushModels(modelShader);
        sceneRenderer->FlushCubemap();

        //Reset camera
        Camera3D::SetPosition(camPos);
        Camera3D::InvertPitch();
        Camera3D::Update();

        EndReflectionFrame();
    }

    void WaterRenderer::RenderRefractionFrame(SceneRenderer* sceneRenderer)
    {
        //Render to refraction framebuffer
        StartRefractionFrame();
        GLRenderSettings::ClearBuffers();

        //Set shader variable(s)
        Shader* terrainShader = sceneRenderer->GetTerrainShader();
        terrainShader->Bind();
        terrainShader->SetUniformVec4f("clipPlane", _refractionClipPlane);
        terrainShader->Unbind();

        //Render scene
        sceneRenderer->FlushTerrain();

        EndRefractionFrame();
    }

    // ----- Public -----

    WaterRenderer::WaterRenderer()
    {
        Logger::Info("Created", "Renderer", __func__);
        InitReflectionFBO();
        InitRefractionFBO();
    }

    void WaterRenderer::Flush(Renderer* sceneRenderer)
    {
        GLRenderSettings::EnableClipDistance(GL_CLIP_DISTANCE0);
        RenderReflectionFrame((SceneRenderer*)sceneRenderer);
        RenderRefractionFrame((SceneRenderer*)sceneRenderer);
        GLRenderSettings::DisableClipDistance(GL_CLIP_DISTANCE0);
    }

    GLTexture* WaterRenderer::GetReflectTexture() const
    {
        return _reflectFBO->GetTexture();
    }

    GLTexture* WaterRenderer::GetRefractTexture() const
    {
        return _refractFBO->GetTexture();
    }

    GLTexture* WaterRenderer::GetRefractDepthTexture() const
    {
        return _refractFBO->GetDepthTexture();
    }
}
