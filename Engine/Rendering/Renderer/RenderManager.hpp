#pragma once

#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"
#include "Renderer.hpp"

#include <vector>

namespace Engine
{
    class RenderManager
    {
        private:
            inline static std::vector<Renderer*> _rendererStorage  = std::vector<Renderer*>();
            inline static bool                   _initialized      = false;

        public:
            RenderManager() = delete;

            static void Init2D();
            static void Init3D();
            static void CleanUp();
            static void PrepareFrame();
            static void Submit(Renderer* renderer);
            static bool GetInitStatus();
    };
}
