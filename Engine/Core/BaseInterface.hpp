#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Types.hpp"
#include "Window.hpp"

namespace Engine
{
    class BaseInterface
    {
        protected:
            static  void SetDarkThemeColors();
            static  void TextCentered(const char* text);

        public:
            BaseInterface();
            virtual ~BaseInterface();
            static  void PrepareFrame();
            virtual void AddElements() = 0;
            static  void Render();
    };
}