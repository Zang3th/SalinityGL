#include "ParticleSimInterface.hpp"

namespace ParticleSim
{
    // ----- Private -----

    void Interface::SetOverlayParameters()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const float  windowWidth       = (float)Engine::WINDOW_WIDTH;
        const float  windowHeight      = (float)Engine::WINDOW_HEIGHT;
        const float  menuBarHeight     = 25.0f;
        const float  sidebarWidth      = 431.0f;

        _sidebarPos      = ImVec2(workPos.x + windowWidth, menuBarHeight);
        _sidebarSize     = ImVec2(sidebarWidth,windowHeight - menuBarHeight);
    }

    // ----- Public -----

    Interface::Interface()
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

    void Interface::AddElements()
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
                    ImGui::Text("Drawcalls total: %d, Vertices total: %d", Engine::Renderer::GetDrawcalls(), Engine::Renderer::GetDrawnVertices());

                    //Profiling/Timing-Results
                    ImGui::NewLine();
                    for(auto const& entry : Engine::Profiler::_results)
                        ImGui::Text("%.3fms - %s", entry.second, entry.first);
                }
                ImGui::End();
            }
        }
    }
}