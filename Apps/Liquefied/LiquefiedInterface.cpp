#include "LiquefiedInterface.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedInterface::AddSideBar() const
    {
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
        ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
        ImGui::SetNextWindowSize(_sidebarSize);

        if(ImGui::Begin("Sidebar", nullptr, _windowFlags))
        {
            // --- Application stats
            ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime_msec(), Engine::Window::GetFps());
            ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime_msec(), 0.0f, 30.0f);

            // --- Render stats
            ImGui::NewLine();
            ImGui::Separator();
            ImGui::Text("Draw calls:     %d", Engine::RenderStatistics::drawCalls);
            ImGui::Text("Drawn vertices: %d", Engine::RenderStatistics::drawnVertices);
            ImGui::Separator();

            // --- Profiling/Timing-Results
            ImGui::NewLine();
            ImGui::Separator();
            for(auto const& entry : Engine::Profiler::results)
            {
                ImGui::Text("%.3fms - %s", entry.second, entry.first);
            }
            ImGui::Separator();

            // --- Numerical value monitoring
            ImGui::NewLine();
            ImGui::Separator();
            CenterText("Numerical value monitoring");
            ImGui::Separator();
            ImGui::NewLine();
            for(auto const& entry : Engine::Monitor::values)
            {
                ImGui::Text("\t%s (min): %5.3f", entry.first, entry.second.min);
                ImGui::Text("\t%s (max): %5.3f", entry.first, entry.second.max);
                ImGui::Text("\t%s (val): %5.3f", entry.first, entry.second.val);
                ImGui::NewLine();
            }
            ImGui::Separator();
        }
        ImGui::End();
    }

    void LiquefiedInterface::AddBufferBar() const
    {
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
        ImGui::SetNextWindowPos(_bufferBarPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(_bufferBarSize);

        if(ImGui::Begin("Bufferbar", nullptr, _windowFlags))
        {
            ImGui::SetCursorPosX(10.0f);
            ImGui::SetCursorPosY(10.0f);
            ImGui::Checkbox("Wireframe Rendering  (W) |", &Engine::UIParams::wireframeRendering);
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(10.0f);
            ImGui::Checkbox("Pause Simulation (SPACE) |", &Engine::LiquiefiedParams::pauseSimulation);

            ImGui::SetCursorPosY(10.0f);
            ImGui::SetCursorPosX(275.0f);
            ImGui::Checkbox("Visualize Smoke   (S) |", &Engine::LiquiefiedParams::visualizeSmoke);
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(275.0f);
            ImGui::Checkbox("Scientific Colors (C) |", &Engine::LiquiefiedParams::scientificColorScheme);

            ImGui::SetCursorPosY(10.0f);
            ImGui::SetCursorPosX(515.0f);
            if(ImGui::Button("Reset Simulation (R)"))
            {
                Engine::LiquiefiedParams::resetSimulation = true;
            }
            ImGui::SameLine();
            ImGui::Text("        |");
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(515.0f);
            ImGui::Text("Turbine power:\t");
            ImGui::SameLine();
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(640.0f);
            ImGui::PushItemWidth(125.0f);
            Input_u32("##Input1", &Engine::LiquiefiedParams::turbinePower, 10, 100, ImGuiInputTextFlags_CharsDecimal);
            ImGui::SameLine();
            ImGui::Text("|");
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
        _menuBarHeight = 0.0f; //Deactivates menu bar for this application
        _windowAlphaValue = 1.0f;
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

        AddBufferBar();
        AddSideBar();

        ImGui::PopStyleVar();
    }
}