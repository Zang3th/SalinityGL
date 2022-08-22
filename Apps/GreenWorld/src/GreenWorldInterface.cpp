#include "GreenWorldInterface.hpp"

namespace GreenWorld
{
    // ----- Private -----

    void Interface::SetOverlayParameters()
    {
        const ImGuiViewport* viewport  = ImGui::GetMainViewport();
        const ImVec2 workPos           = viewport->WorkPos;
        const float  windowWidth       = (float)Engine::WINDOW_WIDTH;
        const float  windowHeight      = (float)Engine::WINDOW_HEIGHT;
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
                ImGui::MenuItem("Show debug sprites", "", &Engine::DEBUG_SPRITES);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::WIREFRAME_RENDERING);
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
                    ImGui::Text("Drawcalls total: %d", Engine::Renderer::GetDrawcalls());
                    ImGui::Text("Vertices  total: %d", Engine::Renderer::GetDrawnVertices());
                    ImGui::Separator();
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Model    render passes: %d", Engine::Renderer::GetModelRenderPasses());
                    ImGui::Text("Terrain  render passes: %d", Engine::Renderer::GetTerrainRenderPasses());
                    ImGui::Text("Water    render passes: %d", Engine::Renderer::GetWaterRenderPasses());
                    ImGui::Text("Particle render passes: %d", Engine::Renderer::GetParticleRenderPasses());
                    ImGui::Text("Sprite   render passes: %d", Engine::Renderer::GetSpriteRenderPasses());
                    ImGui::Text("Cubemap  render passes: %d", Engine::Renderer::GetCubemapRenderPasses());
                    ImGui::Separator();

                    //Camera stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Camera-Position:  (%.1f, %.1f, %.1f)", Engine::Camera::GetPosition().x, Engine::Camera::GetPosition().y, Engine::Camera::GetPosition().z);
                    ImGui::Text("Camera-Front:     (%.1f, %.1f, %.1f)", Engine::Camera::GetFront().x, Engine::Camera::GetFront().y, Engine::Camera::GetFront().z);
                    ImGui::Text("Camera-Yaw/Pitch: (%.2f / %.2f)", Engine::Camera::GetYaw(), Engine::Camera::GetPitch());
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