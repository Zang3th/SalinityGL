#pragma once

#include "Window.hpp"

namespace Engine
{
    class App
    {
        protected:
            virtual void LoadResources() = 0;
            virtual void InitModules()   = 0;

        public:
            virtual void Update() = 0;
            static  bool IsRunning();
    };
}