#pragma once

#include "Window.hpp"
#include "Types.hpp"

namespace Engine
{
    class App
    {
        protected:
            bool _initSuccess = false;

            virtual void LoadResources() = 0;
            virtual uint32 InitModules() = 0;

        public:
            virtual ~App()               = 0;
            virtual void Update()        = 0;
            virtual void ProcessInput()  = 0;

            [[nodiscard]] bool GetInitSuccess() const;
            [[nodiscard]] bool IsRunning() const;
    };
}