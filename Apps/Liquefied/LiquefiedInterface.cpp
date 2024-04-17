#include "LiquefiedInterface.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedInterface::CalculateSidebarDimensions()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const auto   windowWidth       = (float)Engine::WindowParams::WIDTH;
        const auto   windowHeight      = (float)Engine::WindowParams::HEIGHT;

        _sidebarPos  = ImVec2(workPos.x + windowWidth, _menuBarHeight);
        _sidebarSize = ImVec2(_sidebarWidth,windowHeight - _menuBarHeight);
    }

    void LiquefiedInterface::AddMenuBar()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &_showOverlay);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::WindowParams::wireframeRendering);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }
        }
        ImGui::EndMainMenuBar();
    }

    void LiquefiedInterface::AddSideBar()
    {
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
        ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
        ImGui::SetNextWindowSize(_sidebarSize);

        if(ImGui::Begin("Sidebar", nullptr, _windowFlags))
        {
            // --- Stats
            {
                // --- Application stats
                ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime() * 1000.0f, Engine::Window::GetFps());
                ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

                // --- Render stats
                ImGui::Separator();
                ImGui::Text("Draw calls:     %d", Engine::RenderStatistics::drawCalls);
                ImGui::Text("Drawn vertices: %d", Engine::RenderStatistics::drawnVertices);

                // --- Profiling/Timing-Results
                ImGui::Separator();
                for(auto const& entry : Engine::Profiler::_results)
                {
                    ImGui::Text("%.3fms - %s", entry.second, entry.first);
                }
            }
        }
        ImGui::End();
    }

    // ----- Public -----

    LiquefiedInterface::LiquefiedInterface()
        :   _windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                         ImGuiWindowFlags_NoNav        | ImGuiWindowFlags_NoMove),
            _overlayPivot(ImVec2(1.0f, 0.0f)), _windowAlphaValue(0.5f), _menuBarHeight(25.0f), _sidebarWidth(415.0f), _showOverlay(true)
    {
        // --- Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);

        // --- Set style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();

        // --- Calculate overlay parameters
        CalculateSidebarDimensions();
    }

    void LiquefiedInterface::AddElements()
    {
        // --- Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
        {
            ImGui::PlotVarFlushOldEntries();
        }

        // --- Remove all window borders
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // --- Menu bar
        AddMenuBar();

        // --- Add more space between all items
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

        // --- Overlay/Sidebar
        if(_showOverlay)
        {
            AddSideBar();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }
}