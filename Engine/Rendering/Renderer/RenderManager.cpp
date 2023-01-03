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
        _renderStats.Reset();
    }

    void RenderManager::ClearBuffers()
    {
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    RenderStats* RenderManager::GetStats()
    {
        return &_renderStats;
    }

    SceneRenderer* RenderManager::AddScene(const float nearPlane, const float farPlane, const glm::vec3 lightPos, const glm::vec3 lightCol)
    {
        _sceneRenderer = new SceneRenderer(nearPlane, farPlane, lightPos, lightCol);
        _rendererStorage.push_back(_sceneRenderer);

        return _sceneRenderer;
    }

    void RenderManager::RenderScene()
    {
        _sceneRenderer->Flush();
    }
}