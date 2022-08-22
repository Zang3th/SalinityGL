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

    }

    void App::InitModules()
    {
        //Logging
        Engine::Logger::Init();

        //Window
        Engine::Window::Init("ParticleSimulator Demo Application");

        //UI
        _interface = Engine::MakeScope<Interface>();
    }

    // ----- Public -----

    App::App()
    {
        InitModules();
    }

    App::~App()
    {

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
            Engine::Renderer::ClearBuffers();
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