#pragma once

#include "Types.hpp"
#include "ErrorManager.hpp"

namespace Engine
{
    class RenderManager
    {
        private:
            inline static RenderStats _renderStats = RenderStats();

        public:
            RenderManager() = delete;

            static void PrepareFrame();
            static void ClearBuffers();

            static RenderStats* GetStats();
    };
}