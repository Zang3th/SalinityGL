#include "SceneRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SceneRenderer::SceneRenderer(const float nearPlane, const float farPlane, const glm::vec3 lightPos, const glm::vec3 lightCol)
        : _nearPlane(nearPlane), _farPlane(farPlane), _lightPos(lightPos), _lightCol(lightCol),
          _perspProjection(glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, _nearPlane, _farPlane))
    {

    }

    void SceneRenderer::FlushCubemap()
    {
        if(_cubemap)
        {
            bool wireframeWasActive = false;

            if(WIREFRAME_RENDERING)
            {
                wireframeWasActive = true;
                GLRenderSettings::DisableWireframe();
            }

            auto renderStats = RenderManager::GetStats();
            renderStats->drawnVertices += _cubemap->Draw(_perspProjection, Camera3D::GetViewMatrix());
            renderStats->drawCalls++;
            renderStats->cubemapPasses++;

            if(wireframeWasActive)
                GLRenderSettings::EnableWireframe();
        }
    }

    // ----- Public -----

    void SceneRenderer::Flush()
    {
        FlushCubemap();
    }

    void SceneRenderer::AddCubemap(const std::array<const char*, 6>& faces, const std::string& shader)
    {
        _cubemap = MakeScope<Cubemap>(faces, ResourceManager::GetShader(shader));
    }
}