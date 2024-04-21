#include "CellSimInterface.hpp"

namespace CS
{
    // ----- Private -----

    void CellSimInterface::AddMenuBar() const
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &Engine::UIParams::showOverlay);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::UIParams::wireframeRendering);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }

            if(ImGui::BeginMenu("Camera"))
            {
                ImGui::MenuItem("Reset", "", &Engine::UIParams::resetCamera);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }
        }
        ImGui::EndMainMenuBar();
    }

    void CellSimInterface::AddSideBar()
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

                ImGui::Separator();
                ImGui::Text("Model passes:  %d", Engine::RenderStatistics::modelPasses);
                ImGui::Text("Sprite passes: %d", Engine::RenderStatistics::spritePasses);
                ImGui::Text("Cell passes:   %d", Engine::RenderStatistics::cellPasses);

                // --- Camera stats
                ImGui::Separator();
                ImGui::Text("Camera-Position:   ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetPosition().x, Engine::Camera3D::GetPosition().y, Engine::Camera3D::GetPosition().z);
                ImGui::Text("Camera-Front:      ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetFront().x, Engine::Camera3D::GetFront().y, Engine::Camera3D::GetFront().z);
                ImGui::Text("Camera-Yaw, Pitch: ( %.2f, %.2f )", Engine::Camera3D::GetYaw(), Engine::Camera3D::GetPitch());

                // --- Profiling/Timing-Results
                ImGui::Separator();
                for(auto const& entry : Engine::Profiler::_results)
                {
                    ImGui::Text("%.3fms - %s", entry.second, entry.first);
                }
            }

            // --- Cell management
            {
                // --- Titlebar
                ImGui::Separator();
                CenterText("Cell management");
                ImGui::Separator();

                // --- General information
                ImGui::Text("Frame dimensions: (%d, %d, %d)",
                            Engine::CellSimParams::CELL_FRAME_SIZE,
                            Engine::CellSimParams::CELL_FRAME_SIZE,
                            Engine::CellSimParams::CELL_FRAME_SIZE);

                ImGui::Text("Cells alive: %d", Engine::CellSimParams::cellsAlive);

                // --- Delete buttons
                ImGui::SetCursorPosX(_sidebarWidth / 6);
                if(ImGui::Button("Delete cells"))
                {
                    Engine::CellSimParams::deleteCells = true;
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(_sidebarWidth / 2);
                if(ImGui::Button("Delete spawners"))
                {
                    Engine::CellSimParams::deleteSpawners = true;
                }
                ImGui::Separator();

                // --- Cell management tabs
                if(ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    // --- Cell spawn menu
                    if(ImGui::BeginTabItem("Creation"))
                    {
                        // --- Cell selection menu
                        std::string cellTypeString = std::string("Cell type: ") + Engine::CellTypeStrings[Engine::CellSimParams::selectedCellType];
                        if(ImGui::BeginMenu(cellTypeString.c_str()))
                        {
                            if(ImGui::MenuItem("Water"))
                            {
                                Engine::CellSimParams::selectedCellType = Engine::CellType::Water;
                            }

                            ImGui::EndMenu();

                            Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
                        }

                        // --- Cell spawn coordinate input
                        float width = ImGui::CalcItemWidth();
                        ImGui::PushItemWidth(width / 1.5f);

                        ImGui::Text("X Coordinate:\t");
                        ImGui::SameLine();
                        Input_u32("##Input1", &Engine::CellSimParams::selectedCellCoords[0], 1, 10, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::Text("Y Coordinate:\t");
                        ImGui::SameLine();
                        Input_u32("##Input2", &Engine::CellSimParams::selectedCellCoords[1], 1, 10, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::Text("Z Coordinate:\t");
                        ImGui::SameLine();
                        Input_u32("##Input3", &Engine::CellSimParams::selectedCellCoords[2], 1, 10, ImGuiInputTextFlags_CharsDecimal);

                        ImGui::PopItemWidth();
                        CheckCellBoundaries();

                        // --- Checkbox to create a continuous spawner
                        ImGui::Checkbox("Continuous spawner", &Engine::CellSimParams::createSpawner);

                        // --- If a spawner should be created reset cell amount selection value
                        if(Engine::CellSimParams::createSpawner)
                        {
                            Engine::CellSimParams::selectedCellAmount = 0;
                        }
                        // --- Else allow manual cell amount selection for single spawns
                        else
                        {
                            ImGui::Text("Amount:\t");
                            ImGui::SameLine();
                            RadioButton_u32("1", &Engine::CellSimParams::selectedCellAmount, 1);
                            ImGui::SameLine();
                            ImGui::Text("\t");
                            ImGui::SameLine();
                            RadioButton_u32("9", &Engine::CellSimParams::selectedCellAmount, 9);
                            ImGui::SameLine();
                            ImGui::Text("\t");
                            ImGui::SameLine();
                            RadioButton_u32("25", &Engine::CellSimParams::selectedCellAmount, 25);
                            ImGui::SameLine();
                            ImGui::Text("\t");
                            ImGui::SameLine();
                            RadioButton_u32("49", &Engine::CellSimParams::selectedCellAmount, 49);
                        }

                        // --- Spawn button
                        ImGui::Text("\t\t\t");
                        ImGui::SameLine();
                        if(ImGui::Button("Spawn"))
                        {
                            Engine::CellSimParams::spawnCell = true;
                        }
                        // --- Kill button
                        ImGui::SameLine();
                        ImGui::Text("\t\t\t");
                        ImGui::SameLine();
                        if(ImGui::Button("Kill"))
                        {
                            Engine::CellSimParams::killCell = true;
                        }

                        ImGui::EndTabItem();
                    }

                    // --- Debug menu
                    if(ImGui::BeginTabItem("Debug"))
                    {
                        // --- Print debug information
                        if(ImGui::Button("Print debug information"))
                        {
                            Engine::CellSimParams::printDebug = true;
                        }

                        // --- Checkbox to enable single stepping
                        ImGui::Checkbox("Enable single stepping", &Engine::CellSimParams::enableSingleStepping);

                        // --- Check if single stepping is enabled
                        if(Engine::CellSimParams::enableSingleStepping)
                        {
                            if(ImGui::Button("Single Step"))
                            {
                                Engine::CellSimParams::performSingleStep = true;
                            }
                        }

                        ImGui::Separator();

                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            }
        }
        ImGui::End();
    }

    void CellSimInterface::CheckCellBoundaries()
    {
        for(Engine::uint8 i = 0; i < 3; i++)
        {
            if(Engine::CellSimParams::selectedCellCoords[i] >= Engine::CellSimParams::CELL_FRAME_SIZE)
            {
                Engine::CellSimParams::selectedCellCoords[i] = Engine::CellSimParams::CELL_FRAME_SIZE - 1;
            }
        }
    }

    // ----- Public -----

    CellSimInterface::CellSimInterface()
    {
        InitUI();
    }

    void CellSimInterface::InitUI()
    {
        LoadCustomFont("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);
        SetDarkThemeColors();
        CalcElementSizes();
    }

    void CellSimInterface::AddElements()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        //Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
        {
            ImGui::PlotVarFlushOldEntries();
        }

        AddMenuBar();

        //Add more space between all items
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

        if(Engine::UIParams::showOverlay)
        {
            AddSideBar();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }
}