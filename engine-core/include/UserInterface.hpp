#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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
            virtual ~UserInterface();
            static void PrepareFrame();
            virtual void AddElements() = 0;
            static void Render();
    };
}