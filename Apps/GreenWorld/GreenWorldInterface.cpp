#include "GreenWorldInterface.hpp"

namespace GW
{
    // ----- Private -----

    void GreenWorldInterface::AddMenuBar() const
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &Engine::UIParams::showOverlay);
                ImGui::MenuItem("Show debug sprites", "", &Engine::UIParams::debugSprites);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Engine::UIParams::wireframeRendering);
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }

    void GreenWorldInterface::AddSideBar() const
    {
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);
        ImGui::SetNextWindowPos(_sidebarPos, ImGuiCond_Always, _overlayPivot);
        ImGui::SetNextWindowSize(_sidebarSize);

        if(ImGui::Begin("Sidebar", nullptr, _windowFlags))
        {
            // --- Application stats
            ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", Engine::Window::GetDeltaTime() * 1000.0f, Engine::Window::GetFps());
            ImGui::PlotVar("", (float)Engine::Window::GetDeltaTime() * 1000.0f, 0.0f, 30.0f);

            // --- Render stats
            ImGui::NewLine();
            ImGui::Separator();
            ImGui::Text("Draw calls:     %d", Engine::RenderStatistics::drawCalls);
            ImGui::Text("Drawn vertices: %d", Engine::RenderStatistics::drawnVertices);
            ImGui::Separator();

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::Text("Model passes:    %d", Engine::RenderStatistics::modelPasses);
            ImGui::Text("Terrain passes:  %d", Engine::RenderStatistics::terrainPasses);
            ImGui::Text("Water passes:    %d", Engine::RenderStatistics::waterPasses);
            ImGui::Text("Particle passes: %d", Engine::RenderStatistics::particlePasses);
            ImGui::Text("Sprite passes:   %d", Engine::RenderStatistics::spritePasses);
            ImGui::Text("Cubemap passes:  %d", Engine::RenderStatistics::cubemapPasses);
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
        ImGui::End();
    }


    void GreenWorldInterface::AddResourceBar() const
    {
        //Shader stuff
        ImGui::SetNextWindowPos(_shaderFieldPos, ImGuiCond_Always, _resourceBarPivot);
        ImGui::SetNextWindowSize(_shaderFieldSize);
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

        if(ImGui::Begin("ShaderWindow", nullptr, _windowFlags))
        {
            CenterText("Shader:");
            ImGui::Separator();
            ImGui::NewLine();
            CenterText(Engine::ResourceManager::OutputShaderStorage().c_str());
        }
        ImGui::End();

        //Texture stuff
        ImGui::SetNextWindowPos(_texFieldPos, ImGuiCond_Always, _resourceBarPivot);
        ImGui::SetNextWindowSize(_texFieldSize);
        ImGui::SetNextWindowBgAlpha(_windowAlphaValue);

        if(ImGui::Begin("TextureWindow", nullptr, _windowFlags))
        {
            ImGui::Separator();
            CenterText("Textures:");
            ImGui::Separator();
            ImGui::NewLine();
            CenterText(Engine::ResourceManager::OutputTextureStorage().c_str());
        }
        ImGui::End();
    }

    void GreenWorldInterface::CalcElementSizes()
    {
        const     ImGuiViewport* viewport     = ImGui::GetMainViewport();
        const     ImVec2         workPos      = viewport->WorkPos;
        constexpr auto           windowWidth  = (float)Engine::WindowParams::WIDTH;
        constexpr auto           windowHeight = (float)Engine::WindowParams::HEIGHT;

        _sidebarPos      = ImVec2(workPos.x + windowWidth, _menuBarHeight);
        _sidebarSize     = ImVec2(_sidebarWidth,windowHeight - _menuBarHeight);
        _shaderFieldPos  = ImVec2(0.0f, _menuBarHeight);
        _shaderFieldSize = ImVec2(_resourceBarWidth, (windowHeight / 2.0f) - _menuBarHeight);
        _texFieldPos     = ImVec2(_shaderFieldPos.x, _shaderFieldPos.y + _shaderFieldSize.y);
        _texFieldSize    = ImVec2(_shaderFieldSize.x, windowHeight / 2.0f);
    }

    // ----- Public -----

    GreenWorldInterface::GreenWorldInterface()
    {
        InitUI();
    }

    void GreenWorldInterface::InitUI()
    {
        LoadCustomFont("../Res/Assets/Fonts/JetBrainsMono-Medium.ttf", 19);
        SetDarkThemeColors();
        CalcElementSizes();
        _windowAlphaValue = 0.65;
    }

    void GreenWorldInterface::AddElements()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        //Discard old plotting data every 120 frames
        if(Engine::Window::GetFrameCounter() > 120)
        {
            ImGui::PlotVarFlushOldEntries();
        }

        AddMenuBar();

        if(Engine::UIParams::showOverlay)
        {
            AddSideBar();
            AddResourceBar();
        }

        ImGui::PopStyleVar();
    }
}