#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CoreDefines.hpp"
#include "Window.hpp"

namespace Engine
{
    class BaseInterface
    {   
        protected:
            const Window* _window;

        public:
            explicit BaseInterface(const Window* window);
            virtual ~BaseInterface();
            static void PrepareFrame();
            virtual void AddElements() = 0;
            static void Render();
    };
}