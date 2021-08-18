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
            const WindowManager* _window;

        public:
            explicit UserInterface(const WindowManager* window);
            ~UserInterface();
            static void PrepareFrame();
            virtual void AddElements() = 0;
            static void Render();
    };
}