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
                //Print indented
                if(entry.first[0] == '#')
                    ImGui::Text("\t%.3fms - %s", entry.second, &entry.first[1]);
                else
                    ImGui::Text("%.3fms - %s", entry.second, entry.first);
            }
            ImGui::Separator();

            // --- Simulation settings
            ImGui::NewLine();
            ImGui::Separator();
            CenterText("Simulation settings");
            ImGui::Separator();
            ImGui::NewLine();
            ImGui::RadioButton("Forward Euler", (int*)&Engine::LiquiefiedParams::integratorChoice, Engine::Integrator::ForwardEuler);
            ImGui::RadioButton("Backward Euler", (int*)&Engine::LiquiefiedParams::integratorChoice, Engine::Integrator::BackwardEuler);
            ImGui::RadioButton("Semi Lagrangian", (int*)&Engine::LiquiefiedParams::integratorChoice, Engine::Integrator::SemiLagrangian);
            ImGui::NewLine();
            ImGui::Text("Turbine power: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(150.0f);
            Input_u32("##Input1", &Engine::LiquiefiedParams::turbinePower, 100, 1000, ImGuiInputTextFlags_CharsDecimal);
            ImGui::NewLine();
            ImGui::Separator();

            // --- Numerical value monitoring
            if(Engine::LiquiefiedParams::activateDebugging)
            {
                ImGui::NewLine();
                ImGui::Separator();
                CenterText("Numerical value monitoring");
                ImGui::Separator();
                ImGui::NewLine();
                ImGui::Text("Pressure    (min): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::minPressure.val,
                            Engine::LiquefiedDebug::minPressure.x, Engine::LiquefiedDebug::minPressure.y);
                ImGui::Text("Pressure    (max): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::maxPressure.val,
                            Engine::LiquefiedDebug::maxPressure.x, Engine::LiquefiedDebug::maxPressure.y);
                ImGui::Text("Divergence  (min): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::minDivergence.val,
                            Engine::LiquefiedDebug::minDivergence.x, Engine::LiquefiedDebug::minDivergence.y);
                ImGui::Text("Divergence  (max): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::maxDivergence.val,
                            Engine::LiquefiedDebug::maxDivergence.x, Engine::LiquefiedDebug::maxDivergence.y);
                ImGui::Text("u-Advection (min): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::minUAdvect.val,
                            Engine::LiquefiedDebug::minUAdvect.x, Engine::LiquefiedDebug::minUAdvect.y);
                ImGui::Text("u-Advection (max): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::maxUAdvect.val,
                            Engine::LiquefiedDebug::maxUAdvect.x, Engine::LiquefiedDebug::maxUAdvect.y);
                ImGui::Text("v-Advection (min): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::minVAdvect.val,
                            Engine::LiquefiedDebug::minVAdvect.x, Engine::LiquefiedDebug::minVAdvect.y);
                ImGui::Text("v-Advection (max): %+5.3f at (%d, %d)", Engine::LiquefiedDebug::maxVAdvect.val,
                            Engine::LiquefiedDebug::maxVAdvect.x, Engine::LiquefiedDebug::maxVAdvect.y);
                ImGui::NewLine();
                ImGui::Text("CFL-Condition (max): %+1.1f", Engine::LiquefiedDebug::cflCondition);
                ImGui::NewLine();
                ImGui::Separator();
            }

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
            if(ImGui::Button("Reset Simulation (R)"))
            {
                Engine::LiquiefiedParams::resetSimulation = true;
            }
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(275.0f);
            ImGui::Text("                      |");

            ImGui::SetCursorPosY(10.0f);
            ImGui::SetCursorPosX(485.0f);
            ImGui::Checkbox("Visualize Smoke   (S) |", &Engine::LiquiefiedParams::visualizeSmoke);
            ImGui::SetCursorPosY(45.0f);
            ImGui::SetCursorPosX(485.0f);
            ImGui::Checkbox("Scientific Colors (C) |", &Engine::LiquiefiedParams::scientificColorScheme);

            ImGui::SetCursorPosY(10.0f);
            ImGui::SetCursorPosX(725.0f);
            ImGui::Checkbox("Activate Debugging (A) |", &Engine::LiquiefiedParams::activateDebugging);
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