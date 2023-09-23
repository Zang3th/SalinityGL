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
            _windowAlphaValue(0.65f),
            _showOverlay(true)
    {
        //Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);

        //Set style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();

        //Calculate and set overlay parameters
        SetOverlayParameters();
    }

    void CellSimInterface::AddElements()
    {
        //Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
            ImGui::PlotVarFlushOldEntries();

        //Menu bar
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &_showOverlay);
                ImGui::MenuItem("Show debug sprites", "", &Engine::AppSettings::debugSprites);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::AppSettings::wireframeRendering);
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        //Overlay
        if(_showOverlay)
        {
            SetOverlayParameters();

            //Sidebar
            {
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
                ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
                ImGui::SetNextWindowSize(_sidebarSize);

                if(ImGui::Begin("Sidebar", &_showOverlay, _windowFlags))
                {
                    //Application stats
                    ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime() * 1000.0f, Engine::Window::GetFps());
                    ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

                    //Render stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("DrawCalls:      %d", Engine::AppSettings::renderStats.drawCalls);
                    ImGui::Text("DrawnVertices:  %d", Engine::AppSettings::renderStats.drawnVertices);
                    ImGui::Separator();

                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Model passes: %d", Engine::AppSettings::renderStats.modelPasses);
                    ImGui::Text("Cell passes: %d", Engine::AppSettings::renderStats.cellPasses);
                    ImGui::Separator();

                    //Camera stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Camera-Position:   ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetPosition().x, Engine::Camera3D::GetPosition().y, Engine::Camera3D::GetPosition().z);
                    ImGui::Text("Camera-Front:      ( %.1f, %.1f, %.1f )", Engine::Camera3D::GetFront().x, Engine::Camera3D::GetFront().y, Engine::Camera3D::GetFront().z);
                    ImGui::Text("Camera-Yaw, Pitch: ( %.2f, %.2f )", Engine::Camera3D::GetYaw(), Engine::Camera3D::GetPitch());
                    ImGui::Separator();

                    //Profiling/Timing-Results
                    ImGui::NewLine();
                    ImGui::Separator();
                    for(auto const& entry : Engine::Profiler::_results)
                        ImGui::Text("%.3fms - %s", entry.second, entry.first);
                    ImGui::Separator();
                }
                ImGui::End();
            }
        }
    }
}