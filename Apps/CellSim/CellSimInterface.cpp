#include "CellSimInterface.hpp"

namespace CS
{
    // ----- Private -----

    void CellSimInterface::CalculateSidebarDimensions()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const auto   windowWidth       = (float)Engine::AppSettings::WINDOW_WIDTH;
        const auto   windowHeight      = (float)Engine::AppSettings::WINDOW_HEIGHT;

        _sidebarPos  = ImVec2(workPos.x + windowWidth, _menuBarHeight);
        _sidebarSize = ImVec2(_sidebarWidth,windowHeight - _menuBarHeight);
    }

    void CellSimInterface::AddMenuBar()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &_showOverlay);
                ImGui::MenuItem("Show debug sprites", "", &Engine::AppSettings::debugSprites);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::AppSettings::wireframeRendering);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }

            if(ImGui::BeginMenu("Camera"))
            {
                ImGui::MenuItem("Reset", "", &Engine::AppSettings::resetCamera);
                ImGui::EndMenu();
                Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
            }
        }
        ImGui::EndMainMenuBar();
    }

    void CellSimInterface::CheckCellBoundaries()
    {
        for(Engine::uint8 i = 0; i < 3; i++)
        {
            if(Engine::AppSettings::selectedCellCoords[i] > Engine::AppSettings::CELL_FRAME_SIZE)
            {
                Engine::AppSettings::selectedCellCoords[i] = Engine::AppSettings::CELL_FRAME_SIZE;
            }
        }
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
                ImGui::NewLine();
                ImGui::Separator();
                ImGui::Text("Draw calls:     %d", Engine::AppSettings::renderStats.drawCalls);
                ImGui::Text("Drawn vertices: %d", Engine::AppSettings::renderStats.drawnVertices);
                ImGui::Separator();

                ImGui::NewLine();
                ImGui::Separator();
                ImGui::Text("Model passes:  %d", Engine::AppSettings::renderStats.modelPasses);
                ImGui::Text("Sprite passes: %d", Engine::AppSettings::renderStats.spritePasses);
                ImGui::Text("Cell passes:   %d", Engine::AppSettings::renderStats.cellPasses);
                ImGui::Separator();

                // --- Camera stats
                ImGui::NewLine();
                ImGui::Separator();
                ImGui::Text("Camera-Position:   ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetPosition().x, Engine::Camera3D::GetPosition().y, Engine::Camera3D::GetPosition().z);
                ImGui::Text("Camera-Front:      ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetFront().x, Engine::Camera3D::GetFront().y, Engine::Camera3D::GetFront().z);
                ImGui::Text("Camera-Yaw, Pitch: ( %.2f, %.2f )", Engine::Camera3D::GetYaw(), Engine::Camera3D::GetPitch());
                ImGui::Separator();

                // --- Profiling/Timing-Results
                ImGui::NewLine();
                ImGui::Separator();
                for(auto const& entry : Engine::Profiler::_results)
                {
                    ImGui::Text("%.3fms - %s", entry.second, entry.first);
                }
                ImGui::Separator();
            }

            // --- Cell spawn menu
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

                // --- Titlebar
                ImGui::NewLine();
                ImGui::Separator();
                CenterText("Cell management panel");
                ImGui::Separator();

                // --- General information
                ImGui::Text("Frame dimensions: (%d, %d, %d)", Engine::AppSettings::CELL_FRAME_SIZE, Engine::AppSettings::CELL_FRAME_SIZE, Engine::AppSettings::CELL_FRAME_SIZE);
                ImGui::Text("Cells alive: %d", Engine::AppSettings::cellsAlive);
                ImGui::Separator();

                // --- Cell selection menu
                std::string cellTypeString = std::string("Selected cell type: ") + Engine::CellTypeStrings[Engine::AppSettings::selectedCellType];
                if(ImGui::BeginMenu(cellTypeString.c_str()))
                {
                    if(ImGui::MenuItem("None"))
                    {
                        Engine::AppSettings::selectedCellType = Engine::CellType::None;
                    }

                    if(ImGui::MenuItem("Water"))
                    {
                        Engine::AppSettings::selectedCellType = Engine::CellType::Water;
                    }

                    if(ImGui::MenuItem("Lava"))
                    {
                        Engine::AppSettings::selectedCellType = Engine::CellType::Lava;
                    }

                    ImGui::EndMenu();

                    Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
                }

                // --- Cell spawn coordinate input
                float width = ImGui::CalcItemWidth();
                ImGui::PushItemWidth(width / 1.5f);

                ImGui::Text("X Coordinate:\t");
                ImGui::SameLine();
                Input_u32("##1", &Engine::AppSettings::selectedCellCoords[0], 1, 10, ImGuiInputTextFlags_CharsDecimal);
                ImGui::Text("Y Coordinate:\t");
                ImGui::SameLine();
                Input_u32("##2", &Engine::AppSettings::selectedCellCoords[1], 1, 10, ImGuiInputTextFlags_CharsDecimal);
                ImGui::Text("Z Coordinate:\t");
                ImGui::SameLine();
                Input_u32("##3", &Engine::AppSettings::selectedCellCoords[2], 1, 10, ImGuiInputTextFlags_CharsDecimal);

                ImGui::PopItemWidth();
                CheckCellBoundaries();

                // --- Cell spawn amount
                ImGui::Text("Amount:\t");
                ImGui::SameLine();
                RadioButton_u32("1", &Engine::AppSettings::selectedCellAmount, 1);
                ImGui::SameLine();
                ImGui::Text("\t");
                ImGui::SameLine();
                RadioButton_u32("9", &Engine::AppSettings::selectedCellAmount, 9);
                ImGui::SameLine();
                ImGui::Text("\t");
                ImGui::SameLine();
                RadioButton_u32("25", &Engine::AppSettings::selectedCellAmount, 25);
                ImGui::SameLine();
                ImGui::Text("\t");
                ImGui::SameLine();
                RadioButton_u32("49", &Engine::AppSettings::selectedCellAmount, 49);

                // --- Spawn button
                ImGui::Text("\t\t");
                ImGui::SameLine();
                if(ImGui::Button("Spawn"))
                {
                    Engine::AppSettings::spawnNewCell = true;
                }
                ImGui::SameLine();

                // --- Delete all cells
                ImGui::Text("\t\t");
                ImGui::SameLine();
                if(ImGui::Button("Delete all"))
                {
                    Engine::AppSettings::deleteAllCells = true;
                }
                ImGui::Separator();

                //ToDo: Add "generate stone ground" Button

                ImGui::PopStyleVar();
            }
        }
        ImGui::End();
    }

    // ----- Public -----

    CellSimInterface::CellSimInterface()
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

    void CellSimInterface::AddElements()
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

        // --- Overlay/Sidebar
        if(_showOverlay)
        {
            AddSideBar();
        }

        ImGui::PopStyleVar();
    }
}