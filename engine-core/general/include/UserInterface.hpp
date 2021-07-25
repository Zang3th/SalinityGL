#pragma once

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "CoreDefines.hpp"
#include "WindowManager.hpp"

namespace Core
{
    class UserInterface
    {   
        protected:
            Ref<WindowManager> _window;

        public:
            UserInterface(Ref<WindowManager> window);
            ~UserInterface();
            void PrepareFrame();
            virtual void AddElements() = 0;
            void Render();
    };
}