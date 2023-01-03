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

        delete _shadowRenderer;
    }

    void RenderManager::PrepareFrame()
    {
        RENDER_STATS.Reset();
    }

    void RenderManager::ClearBuffers()
    {
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

        return _shadowRenderer;
    }

    void RenderManager::RenderScene()
    {
        _sceneRenderer->Flush();
    }

    void RenderManager::RenderShadows()
    {
        _shadowRenderer->RenderToFramebuffer();
    }
}