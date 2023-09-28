#include "GreenWorldInterface.hpp"

namespace GW
{
    // ----- Private -----

    void GreenWorldInterface::CalculateSidebarDimensions()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const auto   windowWidth       = (float)Engine::AppSettings::WINDOW_WIDTH;
        const auto   windowHeight      = (float)Engine::AppSettings::WINDOW_HEIGHT;

        _sidebarPos  = ImVec2(workPos.x + windowWidth, _menuBarHeight);
        _sidebarSize = ImVec2(_sidebarWidth,windowHeight - _menuBarHeight);
        _shaderFieldPos  = ImVec2(0.0f, _menuBarHeight);
        _shaderFieldSize = ImVec2(_shaderTexBarWidth, (windowHeight / 2.0f) - _menuBarHeight);
        _texFieldPos     = ImVec2(_shaderFieldPos.x, _shaderFieldPos.y + _shaderFieldSize.y);
        _texFieldSize    = ImVec2(_shaderFieldSize.x, windowHeight / 2.0f);
    }

    // ----- Public -----

    GreenWorldInterface::GreenWorldInterface()
        :   _windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                         ImGuiWindowFlags_NoNav        | ImGuiWindowFlags_NoMove),
            _overlayPivot(ImVec2(1.0f, 0.0f)), _shaderPivot(ImVec2(0.0f, 0.0f)), _texturePivot(ImVec2(0.0f, 0.0f)),
            _windowAlphaValue(0.65f), _menuBarHeight(25.0f), _sidebarWidth(415.0f), _shaderTexBarWidth(210.0f), _showOverlay(true)
    {
        //Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);

        //Set style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();

        //Calculate and set overlay parameters
        CalculateSidebarDimensions();
    }

    void GreenWorldInterface::AddElements()
    {
        //Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
            ImGui::PlotVarFlushOldEntries();

        // --- Remove all window borders
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

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
            //Sidebar
            {
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
                ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
                ImGui::SetNextWindowSize(_sidebarSize);

                if(ImGui::Begin("Sidebar", nullptr, _windowFlags))
                {
                    //Application stats
                    ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime() * 1000.0f, Engine::Window::GetFps());
                    ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

                    //Render stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Draw calls:     %d", Engine::AppSettings::renderStats.drawCalls);
                    ImGui::Text("Drawn vertices: %d", Engine::AppSettings::renderStats.drawnVertices);
                    ImGui::Separator();

                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Model passes:    %d", Engine::AppSettings::renderStats.modelPasses);
                    ImGui::Text("Terrain passes:  %d", Engine::AppSettings::renderStats.terrainPasses);
                    ImGui::Text("Water passes:    %d", Engine::AppSettings::renderStats.waterPasses);
                    ImGui::Text("Particle passes: %d", Engine::AppSettings::renderStats.particlePasses);
                    ImGui::Text("Sprite passes:   %d", Engine::AppSettings::renderStats.spritePasses);
                    ImGui::Text("Cubemap passes:  %d", Engine::AppSettings::renderStats.cubemapPasses);
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

            //ShaderWindow
            {
                ImGui::SetNextWindowPos(_shaderFieldPos, ImGuiCond_Always, _shaderPivot);
                ImGui::SetNextWindowSize(_shaderFieldSize);
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

                if(ImGui::Begin("ShaderWindow", &_showOverlay, _windowFlags))
                {
                    CenterText("Shader:");
                    ImGui::Separator();
                    ImGui::NewLine();
                    CenterText(Engine::ResourceManager::OutputShaderStorage().c_str());
                }
                ImGui::End();
            }

            //TextureWindow
            {
                ImGui::SetNextWindowPos(_texFieldPos, ImGuiCond_Always, _texturePivot);
                ImGui::SetNextWindowSize(_texFieldSize);
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

                if(ImGui::Begin("TextureWindow", &_showOverlay, _windowFlags))
                {
                    ImGui::Separator();
                    CenterText("Textures:");
                    ImGui::Separator();
                    ImGui::NewLine();
                    CenterText(Engine::ResourceManager::OutputTextureStorage().c_str());
                }
                ImGui::End();
            }
        }

        ImGui::PopStyleVar();
    }
}