#include "GW_App.hpp"

namespace GW
{
    // ----- Private -----

    void App::ConfigureLogger()
    {
        //Add colorful terminal logging
        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

        //Configure logger
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s} [%level] [%fbase] %msg");
        el::Loggers::reconfigureLogger("default", defaultConf);
    }

    void App::LoadResources()
    {
        _resourceManager.LoadTexture("StoneTexture", "../res/textures/greenWorld/Stone.jpg");
        _resourceManager.LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");
    }

    // ----- Public -----

    App::App()
    {
        //Configure logger
        ConfigureLogger();

        //Window-Management
        _windowManager = Core::MakeScope<Core::WindowManager>();
        _windowManager->SetWindowTitle("GreenWorld Demo Application");

        //3D-Camera
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 15.0f);

        //Input-Management
        InputManager::Init(_windowManager.get(), _camera.get());

        //Renderer
        _renderer = Core::MakeScope<Core::Renderer>();

        //UI
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get());

        //Resources
        LoadResources();

        //Create test sprite
        _testSprite = Core::MakeScope<Core::Sprite>
        (
            _resourceManager.GetTexture("StoneTexture"),
            _resourceManager.GetShader("SpriteShader"),
            glm::vec2(0.0f, 150.0f),
            glm::vec2(800.0f, 800.0f),
            0.0f,
            glm::vec3(0.37f, 0.77, 0.29f)
       );

        //Initialize profiler results storage map
        Core::ProfileResults::_results["Prepare frame"] = 0.0f;
        Core::ProfileResults::_results["Render graphics"] = 0.0f;
        Core::ProfileResults::_results["Render UI"] = 0.0f;
        Core::ProfileResults::_results["Move sprite"] = 0.0f;
        Core::ProfileResults::_results["End frame"] = 0.0f;
    }

    bool App::IsRunning()
    {
        return _windowManager->WindowIsRunning();
    }

    void App::Update()
    {
        //Prepare frame
        {
            Core::PROFILE_SCOPE("Prepare frame");
            _windowManager->PrepareFrame();
            _renderer->Prepare();
            _userInterface->PrepareFrame();
        }

        //Render graphics
        {
            Core::PROFILE_SCOPE("Render graphics");
            _renderer->Submit(_testSprite.get());
            _renderer->Flush();
        }

        //Render UI (always after graphics)
        {
            Core::PROFILE_SCOPE("Render UI");
            _userInterface->AddElements();
            _userInterface->Render();
        }

        //Move sprite
        {
            Core::PROFILE_SCOPE("Move sprite");
            _testSprite->Translate(glm::vec2(30.0f * _windowManager->GetDeltaTime(), 0.0f));
        }

        //End frame
        {
            Core::PROFILE_SCOPE("End frame");
            _windowManager->SwapBuffers();
        }
    }
}