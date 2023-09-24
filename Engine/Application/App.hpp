#pragma once

#include "Window.hpp"
#include "Types.hpp"

namespace Engine
{
    class App
    {
        protected:
            virtual void LoadResources() = 0;
            virtual uint32 InitModules() = 0;

        public:
            bool appStartSuccess;
            virtual ~App() = 0;
            virtual void Update() = 0;
            static bool IsRunning();
    };
}