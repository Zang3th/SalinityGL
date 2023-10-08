#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_plot_var.h"
#include "Types.hpp"
#include "Window.hpp"

namespace Engine
{
    class Interface
    {
        protected:
            static void SetDarkThemeColors();
            static void CenterText(const char* text);
            static void CenterCursor(const char* characterAmount);
            static bool RadioButton_u32(const char* label, uint32* v, uint32 v_button);
            static bool Input_u32(const char* label, uint32* v, uint32 step, uint32 step_fast, ImGuiInputTextFlags flags);

        public:
            Interface();
            virtual ~Interface();
            virtual void AddElements() = 0;
            static  void PrepareFrame();
            static  void Render();
    };
}