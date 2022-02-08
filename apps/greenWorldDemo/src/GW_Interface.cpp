#include "GW_Interface.hpp"

namespace GW
{
    // ----- Private -----

    void Interface::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    void Interface::TextCentered(const char* text)
    {
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth   = ImGui::CalcTextSize(text).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("%s", text);
    }

    void Interface::SetOverlayParameters()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;
        ImVec2 work_size = viewport->WorkSize;

        _sidebarPos = ImVec2(work_size.x, work_pos.y);
        _overlayPivot = ImVec2(1.0f, 0.0f);

        _shaderFieldPos = ImVec2((float)Core::WINDOW_WIDTH - 230.0f, (float)Core::WINDOW_HEIGHT - 460.0f + work_pos.y);
        _texFieldPos = ImVec2(_shaderFieldPos.x + 230.0f, _shaderFieldPos.y);

        _overlaySize = ImVec2(460.0f, (float)Core::WINDOW_HEIGHT - 460.0f);
        _bottomFieldSizes = ImVec2(_overlaySize.x / 2, 460.0f);
    }

    // ----- Public -----

    Interface::Interface(const Core::Window* window)
        :   Core::BaseInterface(window),
            _windowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove),
            _windowAlphaValue(0.65f),
            _showOverlay(true)
    {
        //Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../res/fonts/JetBrainsMono-Medium.ttf", 19);

        //Set style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();

        //Calculate and set overlay parameters
        SetOverlayParameters();
    }

    void Interface::AddElements()
    {
        //Discard old plotting data every 120 frames
        if(_window->GetFrameCounter() > 120)
            ImGui::PlotVarFlushOldEntries();

        //Menu bar
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &_showOverlay);
                ImGui::MenuItem("Show debug sprites", "", &Core::DEBUG_SPRITES);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Core::WIREFRAME_RENDERING);
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
                ImGui::SetNextWindowSize(_overlaySize);

                if(ImGui::Begin("Sidebar", &_showOverlay, _windowFlags))
                {
                    //Application stats
                    ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", _window->GetDeltaTime() * 1000.0f, _window->GetFps());
                    ImGui::PlotVar("", (float)_window->GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

                    //Render stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Drawcalls total:       %d", Core::Renderer::GetDrawcalls());
                    ImGui::Text("Vertices total:        %d", Core::Renderer::GetDrawnVertices());
                    ImGui::Text("Model render passes:   %d", Core::Renderer::GetModelRenderPasses());
                    ImGui::Text("Sprite render passes:  %d", Core::Renderer::GetSpriteRenderPasses());
                    ImGui::Text("Cubemap render passes: %d", Core::Renderer::GetCubemapRenderPasses());
                    ImGui::Separator();

                    //Camera stats
                    ImGui::NewLine();
                    ImGui::Separator();
                    ImGui::Text("Camera-Position:  X: %.1f | Y: %.1f | Z: %.1f", Core::Camera::GetPosition().x, Core::Camera::GetPosition().y, Core::Camera::GetPosition().z);
                    ImGui::Text("Camera-Front:     X: %.1f | Y: %.1f | Z: %.1f", Core::Camera::GetFront().x, Core::Camera::GetFront().y, Core::Camera::GetFront().z);
                    ImGui::Text("Camera-Yaw|Pitch: %.2f | %.2f", Core::Camera::GetYaw(), Core::Camera::GetPitch());
                    ImGui::Separator();

                    //Profiling/Timing-Results
                    ImGui::NewLine();
                    ImGui::Separator();
                    for(auto const& entry : Core::Profiler::_results)
                        ImGui::Text("%.3fms - %s", entry.second, entry.first);
                    ImGui::Separator();
                }
                ImGui::End();
            }

            //ShaderWindow
            {
                ImGui::SetNextWindowPos(_shaderFieldPos, ImGuiCond_Always, _overlayPivot);
                ImGui::SetNextWindowSize(_bottomFieldSizes);
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

                if(ImGui::Begin("ShaderWindow", &_showOverlay, _windowFlags))
                {
                    TextCentered("Shader:");
                    ImGui::NewLine();
                    TextCentered(Core::ResourceManager::OutputShaderStorage().c_str());
                }
                ImGui::End();
            }

            //TextureWindow
            {
                ImGui::SetNextWindowPos(_texFieldPos, ImGuiCond_Always, _overlayPivot);
                ImGui::SetNextWindowSize(_bottomFieldSizes);
                ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

                if(ImGui::Begin("TextureWindow", &_showOverlay, _windowFlags))
                {
                    TextCentered("Textures:");
                    ImGui::NewLine();
                    TextCentered(Core::ResourceManager::OutputTextureStorage().c_str());
                }
                ImGui::End();
            }
        }
    }
}