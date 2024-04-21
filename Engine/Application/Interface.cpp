#include "Interface.hpp"

namespace Engine
{
    // ----- Protected -----

    void Interface::SetDarkThemeColors()
    {
        ImGui::StyleColorsDark();

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Slider
        colors[ImGuiCol_SliderGrab]       = ImVec4{0.20f, 0.25f, 0.55f, 1.0f};
        colors[ImGuiCol_SliderGrabActive] = ImVec4{0.45f, 0.50f, 0.80f, 1.0f};

        // Radio buttons and checkmarks
        colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Interface::CenterText(const char* text)
    {
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth   = ImGui::CalcTextSize(text).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("%s",text);
    }

    void Interface::CenterCursor(const char* characterAmount)
    {
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth   = ImGui::CalcTextSize(characterAmount).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    }

    void Interface::LoadCustomFont(const char* filepath, float size)
    {
        const ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(filepath, size);
    }

    bool Interface::RadioButton_u32(const char* label, uint32* v, uint32 v_button)
    {
        const bool pressed = ImGui::RadioButton(label, *v == v_button);
        if (pressed)
            *v = v_button;
        return pressed;
    }

    bool Interface::Input_u32(const char* label, uint32* v, uint32 step, uint32 step_fast, ImGuiInputTextFlags flags)
    {
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return ImGui::InputScalar(label, ImGuiDataType_S32, (void*)v, (void*)(step > 0 ? &step : nullptr), (void*)(step_fast > 0 ? &step_fast : nullptr), format, flags);
    }

    // ----- Public -----

    Interface::Interface()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    Interface::~Interface()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Interface::CalcElementSizes()
    {
        const     ImGuiViewport* viewport     = ImGui::GetMainViewport();
        const     ImVec2         workPos      = viewport->WorkPos;
        constexpr auto           windowWidth  = (float)Engine::WindowParams::WIDTH;
        constexpr auto           windowHeight = (float)Engine::WindowParams::HEIGHT;

        _sidebarPos  = ImVec2(workPos.x + windowWidth, _menuBarHeight);
        _sidebarSize = ImVec2(_sidebarWidth,windowHeight - _menuBarHeight);
    }

    void Interface::PrepareFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Interface::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}