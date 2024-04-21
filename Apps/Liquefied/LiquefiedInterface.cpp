#include "LiquefiedInterface.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedInterface::AddMenuBar() const
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &Engine::UIParams::showOverlay);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::UIParams::wireframeRendering);
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }

    void LiquefiedInterface::AddSideBar() const
    {
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
        ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
        ImGui::SetNextWindowSize(_sidebarSize);

        if(ImGui::Begin("Sidebar", nullptr, _windowFlags))
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
        ImGui::End();
    }

    // ----- Public -----

    LiquefiedInterface::LiquefiedInterface()
    {
        InitUI();
    }


    void LiquefiedInterface::InitUI()
    {
        LoadCustomFont("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);
        SetDarkThemeColors();
        CalcElementSizes();
    }

    void LiquefiedInterface::AddElements()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        //Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
        {
            ImGui::PlotVarFlushOldEntries();
        }

        AddMenuBar();

        if(Engine::UIParams::showOverlay)
        {
            AddSideBar();
        }

        ImGui::PopStyleVar();
    }
}