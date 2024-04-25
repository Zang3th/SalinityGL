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
            ImGuiWindowFlags _windowFlags  = ImGuiWindowFlags_NoDecoration       |
                                             ImGuiWindowFlags_NoSavedSettings    |
                                             ImGuiWindowFlags_NoFocusOnAppearing |
                                             ImGuiWindowFlags_NoNav              |
                                             ImGuiWindowFlags_NoMove;
            ImVec2           _sidebarPos   = ImVec2(0.0f, 0.0f);
            ImVec2           _sidebarSize  = ImVec2(0.0f, 0.0f);
            ImVec2           _overlayPivot = ImVec2(1.0f, 0.0f);

            static void SetDarkThemeColors();
            static void CenterText(const char* text);
            static void CenterCursor(const char* characterAmount);
            static void LoadCustomFont(const char* filepath, float size);
            static bool RadioButton_u32(const char* label, uint32* v, uint32 v_button);
            static bool Input_u32(const char* label, uint32* v, uint32 step, uint32 step_fast, ImGuiInputTextFlags flags);

        public:
            float _windowAlphaValue = 0.5f;
            float _menuBarHeight    = 25.0f;
            float _sidebarWidth     = 420.0f;

            Interface();
            virtual ~Interface();
            virtual void InitUI()      = 0;
            virtual void AddElements() = 0;
            virtual void CalcElementSizes();
            static  void PrepareFrame();
            static  void Render();
    };
}