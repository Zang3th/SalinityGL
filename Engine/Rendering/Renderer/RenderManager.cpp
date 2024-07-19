#include "RenderManager.hpp"

namespace Engine
{
    // ----- Public -----

    void RenderManager::Init2D()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(10);
        _initialized = true;
    }

    void RenderManager::Init3D()
    {
        _rendererStorage.clear();
        _rendererStorage.reserve(10);

        //OpenGL-Rendersettings
        GLRenderSettings::EnableMultisample();
        GLRenderSettings::EnableDepthtest();
        GLRenderSettings::SetDepthFunc(GL_LEQUAL);
        GLRenderSettings::EnableBlending();
        GLRenderSettings::SetBlendFunc(GL_ONE_MINUS_SRC_ALPHA);
        GLRenderSettings::EnableCulling();
        GLRenderSettings::SetCullFace(GL_BACK);

        _initialized = true;
    }

    void RenderManager::CleanUp()
    {
        for(auto const& renderer : _rendererStorage)
        {
            delete renderer;
        }
    }

    void RenderManager::PrepareFrame()
    {
        RenderStatistics::Reset();
        GLRenderSettings::ClearBuffers();
    }

    void RenderManager::Submit(Renderer *renderer)
    {
        _rendererStorage.push_back(renderer);
    }

    bool RenderManager::GetInitStatus()
    {
        return _initialized;
    }
}
