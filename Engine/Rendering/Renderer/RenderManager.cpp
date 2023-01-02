#include "RenderManager.hpp"

namespace Engine
{
    // ----- Private -----


    // ----- Public -----

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
}