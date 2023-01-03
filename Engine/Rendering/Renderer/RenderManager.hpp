#pragma once

#include "Types.hpp"
#include "ErrorManager.hpp"
#include "glm.hpp"
#include "Renderer.hpp"
#include "SceneRenderer.hpp"

#include <vector>

namespace Engine
{
    class RenderManager
    {
        private:
            inline static RenderStats            _renderStats     = RenderStats();
            inline static std::vector<Renderer*> _rendererStorage = std::vector<Renderer*>();
            inline static SceneRenderer*         _sceneRenderer   = nullptr;

        public:
            RenderManager() = delete;

            static void Init();
            static void CleanUp();
            static void PrepareFrame();
            static void ClearBuffers();
            static RenderStats* GetStats();

            static SceneRenderer* AddScene(float nearPlane, float farPlane, glm::vec3 lightPos, glm::vec3 lightCol);

            static void RenderScene();
    };
}