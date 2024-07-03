#pragma once

#include "Window.hpp"
#include "Types.hpp"

namespace Engine
{
    class App
    {
        protected:
            virtual void LoadResources() = 0;

        public:
            virtual ~App()               = 0;
            virtual void Update()        = 0;
            virtual void ProcessInput()  = 0;
            [[nodiscard]] virtual bool Init() = 0;

            [[nodiscard]] bool IsRunning() const;
    };
}
