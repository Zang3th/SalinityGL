#include "RenderManager.hpp"

namespace Engine
{
    // ----- Public -----

    void RenderManager::Init()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(5);
    }

    void RenderManager::CleanUp()
    {
        for(auto const& renderer : _rendererStorage)
            delete renderer;
    }

    void RenderManager::PrepareFrame()
    {
        RENDER_STATS.Reset();
        GLRenderSettings::ClearBuffers();
    }

    SceneRenderer* RenderManager::AddScene(const float nearPlane, const float farPlane, const glm::vec3 lightPos, const glm::vec3 lightCol)
    {
        _sceneRenderer = new SceneRenderer(nearPlane, farPlane, lightPos, lightCol);
        _rendererStorage.push_back(_sceneRenderer);

        return _sceneRenderer;
    }

    ShadowRenderer* RenderManager::AddShadows(uint32 resolution, glm::vec3 lightPos, const std::string& shader)
    {
        _shadowRenderer = new ShadowRenderer(resolution, resolution, lightPos, ResourceManager::GetShader(shader));
        _rendererStorage.push_back(_shadowRenderer);

        return _shadowRenderer;
    }

    void RenderManager::RenderScene()
    {
        _sceneRenderer->Flush((Renderer*)_shadowRenderer);
    }

    void RenderManager::RenderShadows()
    {
        _shadowRenderer->Flush((Renderer*)_sceneRenderer);
    }
}