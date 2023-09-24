#include "CellSimInterface.hpp"

namespace CS
{
    // ----- Private -----

    void CellSimInterface::SetOverlayParameters()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const auto   windowWidth       = (float)Engine::AppSettings::WINDOW_WIDTH;
        const auto   windowHeight      = (float)Engine::AppSettings::WINDOW_HEIGHT;
        const float  menuBarHeight     = 25.0f;
        const float  sidebarWidth      = 415.0f;

        _sidebarPos      = ImVec2(workPos.x + windowWidth, menuBarHeight);
        _sidebarSize     = ImVec2(sidebarWidth,windowHeight - menuBarHeight);
    }

    // ----- Public -----

    CellSimInterface::CellSimInterface()
        :   _windowFlags(ImGuiWindowFlags_NoDecoration       | ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav           |
                         ImGuiWindowFlags_NoMove),
            _windowAlphaValue(0.85f),
            _showOverlay(true)
    {
        // --- Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);

        // --- Set style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();

        // --- Calculate and set overlay parameters
        SetOverlayParameters();
    }

    void CellSimInterface::AddElements()
    {
        // --- Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
        {
            ImGui::PlotVarFlushOldEntries();
        }

        // --- Menu bar
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

        // --- Overlay
        if(_showOverlay)
        {
            SetOverlayParameters();

            // --- Sidebar
            {
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
                ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
                ImGui::SetNextWindowSize(_sidebarSize);

                if(ImGui::Begin("Sidebar", &_showOverlay, _windowFlags))
                {
                    // --- Application stats
                    ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime() * 1000.0f, Engine::Window::GetFps());
                    ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

                    // --- Render stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("DrawCalls:      %d", Engine::AppSettings::renderStats.drawCalls);
                    ImGui::Text("DrawnVertices:  %d", Engine::AppSettings::renderStats.drawnVertices);
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

                    // --- Cell spawn menu
                    ImGui::NewLine();
                    ImGui::Separator();
                    TextCentered("Cell spawn menu");
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
                    ImGui::Separator();
                    ImGui::Text("Frame dimensions: (%d, %d, %d)", Engine::AppSettings::CELL_FRAME_SIZE, Engine::AppSettings::CELL_FRAME_SIZE, Engine::AppSettings::CELL_FRAME_SIZE);

                    // --- Drop down menu
                    std::string cellTypeString = std::string("Cell type: ") + Engine::CellTypeStrings[Engine::AppSettings::selectedCellType];
                    if(ImGui::BeginMenu(cellTypeString.c_str()))
                    {
                        if(ImGui::MenuItem("None"))
                        {
                            Engine::AppSettings::selectedCellType = Engine::CellTypes::None;
                        }

                        if(ImGui::MenuItem("Water"))
                        {
                            Engine::AppSettings::selectedCellType = Engine::CellTypes::Water;
                        }

                        if(ImGui::MenuItem("Magma"))
                        {
                            Engine::AppSettings::selectedCellType = Engine::CellTypes::Magma;
                        }

                        if(ImGui::MenuItem("Stone"))
                        {
                            Engine::AppSettings::selectedCellType = Engine::CellTypes::Stone;
                        }
                        ImGui::EndMenu();

                        Engine::CameraController3D::DeFocusWindow(Engine::Window::GetWindow());
                    }

                    // --- Coordinate input
                    float width = ImGui::CalcItemWidth();
                    ImGui::PushItemWidth(width / 1.5f);

                    ImGui::Text("X Coordinate:\t");
                    ImGui::SameLine();
                    ImGui::InputInt("##1", &Engine::AppSettings::selectedXCoord, 10, 0, ImGuiInputTextFlags_CharsDecimal);

                    ImGui::Text("Y Coordinate:\t");
                    ImGui::SameLine();
                    ImGui::InputInt("##2", &Engine::AppSettings::selectedYCoord, 10, 0, ImGuiInputTextFlags_CharsDecimal);

                    ImGui::Text("Z Coordinate:\t");
                    ImGui::SameLine();
                    ImGui::InputInt("##3", &Engine::AppSettings::selectedZCoord, 10, 0, ImGuiInputTextFlags_CharsDecimal);

                    //ToDo: Implement boundary checks

                    // --- Spawn button
                    //ToDo: Integrate in base class
                    ImGui::PopItemWidth();
                    auto windowWidth = ImGui::GetWindowSize().x;
                    auto textWidth   = ImGui::CalcTextSize("Spawn").x;

                    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
                    if (ImGui::Button("Spawn"))
                    {
                        //ToDo: Implement spawn logic
                    }
                    ImGui::PopStyleVar();
                    ImGui::Separator();
                }
                ImGui::End();
            }
        }
    }
}