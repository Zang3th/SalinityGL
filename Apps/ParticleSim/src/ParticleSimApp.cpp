#include "ParticleSimApp.hpp"

//Initialize extern settings
const   Engine::uint32 Engine::WINDOW_WIDTH        = 1920;
const   Engine::uint32 Engine::WINDOW_HEIGHT       = 1080;
const   Engine::uint32 Engine::PLANE_SIZE          = 0;
        bool           Engine::WIREFRAME_RENDERING = false;
        bool           Engine::DEBUG_SPRITES       = false;
const   float          Engine::GRAVITY             = 0.0f;

namespace ParticleSim
{
    // ----- Private -----

    void App::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("ParticleBGShader", "../Res/Shader/ParticleSim/ParticleBG_VS.glsl", "../Res/Shader/ParticleSim/ParticleBG_FS.glsl");
    }

    void App::InitModules()
    {
        //Logging
        Engine::Logger::Init();

        //Window
        Engine::Window::Init("ParticleSimulator Demo Application");

        //UI
        _interface = Engine::MakeScope<Interface>();

        //Resources
        LoadResources();

        //Renderer
        Engine::Renderer::Init2DBasic();
    }

    void App::CreateSprites()
    {
        //BackgroundSprite
        _bgSprite = Engine::MakeScope<Engine::Sprite>
        (
            nullptr,
            Engine::ResourceManager::GetShader("ParticleBGShader"),
            glm::vec3(0.48f, 0.55f, 0.57f)
        );

        Engine::Renderer::Submit(_bgSprite.get());
    }

    // ----- Public -----

    App::App()
    {
        InitModules();

        //Call after init because these methods depend on OpenGL-Initialization
        CreateSprites();
    }

    App::~App()
    {
        Engine::ResourceManager::CleanUp();
    }

    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }

    void App::Update()
    {
        {
            Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            _interface->PrepareFrame();
            Engine::Renderer::PrepareFrame();
            Engine::Renderer::ClearBuffers();
        }

        {
            Engine::PROFILE_SCOPE("Render graphics");

            Engine::Renderer::FlushSprites();
        }

        {
            Engine::PROFILE_SCOPE("Render UI");

            _interface->AddElements();
            _interface->Render();
        }

        {
            Engine::PROFILE_SCOPE("End frame");

            Engine::Window::SwapBuffers();
        }
    }
}