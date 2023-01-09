#include "RenderManager.hpp"

namespace Engine
{
    // ----- Public -----

    void RenderManager::Init()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(5);

        //OpenGL-Rendersettings
        GLRenderSettings::EnableMultisample();
        GLRenderSettings::EnableDepthtest();
        GLRenderSettings::SetDepthFunc(GL_LEQUAL);
        GLRenderSettings::EnableBlending();
        GLRenderSettings::SetBlendFunc(GL_ONE_MINUS_SRC_ALPHA);
    }

    void RenderManager::CleanUp()
    {
        for(auto const& renderer : _rendererStorage)
            delete renderer;
    }

    void RenderManager::PrepareFrame()
    {
        APP_SETTINGS.renderStats.Reset();
        GLRenderSettings::ClearBuffers();
    }

    SceneRenderer* RenderManager::AddScene(const float nearPlane, const float farPlane, const glm::vec3& lightPos, const glm::vec3& lightCol)
    {
        _sceneRenderer = new SceneRenderer(nearPlane, farPlane, lightPos, lightCol);
        _rendererStorage.push_back(_sceneRenderer);

        return _sceneRenderer;
    }

    ShadowRenderer* RenderManager::AddShadows(const uint32 resolution, const glm::vec3& lightPos, const std::string& shader)
    {
        _shadowRenderer = new ShadowRenderer(resolution, resolution, lightPos, ResourceManager::GetShader(shader));
        _rendererStorage.push_back(_shadowRenderer);
        _sceneRenderer->AddLightProjection(_shadowRenderer->GetLightProjection());

        return _shadowRenderer;
    }

    SpriteRenderer* RenderManager::AddSprites()
    {
        _spriteRenderer = new SpriteRenderer();
        _rendererStorage.push_back(_spriteRenderer);

        return _spriteRenderer;
    }

    WaterRenderer* RenderManager::AddWater()
    {
        _waterRenderer = new WaterRenderer();
        _rendererStorage.push_back(_waterRenderer);

        return _waterRenderer;
    }

    void RenderManager::RenderScene()
    {
        _sceneRenderer->Flush(nullptr);
    }

    void RenderManager::RenderShadows()
    {
        _shadowRenderer->Flush((Renderer*)_sceneRenderer);
    }

    void RenderManager::RenderSprites()
    {
        _spriteRenderer->Flush(nullptr);
    }

    void RenderManager::RenderWater()
    {
        _waterRenderer->Flush((Renderer*)_sceneRenderer);
    }
}