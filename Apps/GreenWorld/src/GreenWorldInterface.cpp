#include "GreenWorldInterface.hpp"

namespace GW
{
    // ----- Private -----

    void GreenWorldInterface::SetOverlayParameters()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const auto   windowWidth       = (float)Engine::APP_SETTINGS.WINDOW_WIDTH;
        const auto   windowHeight      = (float)Engine::APP_SETTINGS.WINDOW_HEIGHT;
        const float  menuBarHeight     = 25.0f;
        const float  sidebarWidth      = 415.0f;
        const float  shaderTexBarWidth = 210.0f;

        _sidebarPos      = ImVec2(workPos.x + windowWidth, menuBarHeight);
        _sidebarSize     = ImVec2(sidebarWidth,windowHeight - menuBarHeight);
        _shaderFieldPos  = ImVec2(0.0f, menuBarHeight);
        _shaderFieldSize = ImVec2(shaderTexBarWidth, (windowHeight / 2.0f) - menuBarHeight);
        _texFieldPos     = ImVec2(_shaderFieldPos.x, _shaderFieldPos.y + _shaderFieldSize.y);
        _texFieldSize    = ImVec2(_shaderFieldSize.x, windowHeight / 2.0f);
    }

    // ----- Public -----

    GreenWorldInterface::GreenWorldInterface()
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

    void GreenWorldInterface::AddElements()
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
                ImGui::MenuItem("Show debug sprites", "", &Engine::APP_SETTINGS.debugSprites);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::APP_SETTINGS.wireframeRendering);
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
                    ImGui::Text("DrawCalls:      %d", Engine::APP_SETTINGS.renderStats.drawCalls);
                    ImGui::Text("DrawnVertices:  %d", Engine::APP_SETTINGS.renderStats.drawnVertices);
                    ImGui::Separator();
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Model   passes: %d", Engine::APP_SETTINGS.renderStats.modelPasses);
                    ImGui::Text("Terrain passes: %d", Engine::APP_SETTINGS.renderStats.terrainPasses);
                    ImGui::Text("Water   passes: %d", Engine::APP_SETTINGS.renderStats.waterPasses);
                    //ImGui::Text("Particle render passes: %d", Engine::Renderer::GetParticleRenderPasses());
                    ImGui::Text("Sprite  passes: %d", Engine::APP_SETTINGS.renderStats.spritePasses);
                    ImGui::Text("Cubemap passes: %d", Engine::APP_SETTINGS.renderStats.cubemapPasses);
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
                    TextCentered("Shader:");
                    ImGui::NewLine();
                    TextCentered(Engine::ResourceManager::OutputShaderStorage().c_str());
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
                    TextCentered("Textures:");
                    ImGui::NewLine();
                    TextCentered(Engine::ResourceManager::OutputTextureStorage().c_str());
                }
                ImGui::End();
            }
        }
    }
}