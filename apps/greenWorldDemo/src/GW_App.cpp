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

    void App::ConfigureProfiler()
    {
        Core::ProfileResults::AddFunctionScope("Process events");
        Core::ProfileResults::AddFunctionScope("Prepare frame");
        Core::ProfileResults::AddFunctionScope("Render graphics");
        Core::ProfileResults::AddFunctionScope("Render UI");
        Core::ProfileResults::AddFunctionScope("End frame");
    }

    void App::InitModules()
    {
        //Window
        _windowManager = Core::MakeScope<Core::WindowManager>();
        _windowManager->SetWindowTitle("GreenWorld Demo Application");

        //Camera & Renderer
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-11.0f, 17.0f, 9.0f), 0.0f, -37.0f, 15.0f);
        _renderer = Core::MakeScope<Core::Renderer>(_camera.get());

        //Input & UI
        InputManager::Init(_windowManager.get(), _camera.get());
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get(), _camera.get());

        //Resources
        _resourceManager = Core::MakeScope<Core::ResourceManager>();
    }

    void App::LoadResources()
    {
        //Textures
        _resourceManager->LoadTexture("StoneTexture", "../res/textures/greenWorld/Stone.jpg");
        _resourceManager->LoadTexture("BlockTexture", "../res/textures/greenWorld/Block.jpg");
        _resourceManager->LoadTexture("SwordTexture", "../res/models/sword/textures/Object001_mtl_baseColor.jpeg");

        //Shaders
        _resourceManager->LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");
        _resourceManager->LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
    }

    // ----- Public -----

    App::App()
    {
        ConfigureLogger();
        ConfigureProfiler();
        InitModules();
        LoadResources();

        //Create test sprite
        _testSprite = Core::MakeScope<Core::Sprite>
        (
            _resourceManager->GetTexture("StoneTexture"),
            _resourceManager->GetShader("SpriteShader"),
            glm::vec2(1500.0f, 700.0f),
            glm::vec2(300.0f, 300.0f),
            0.0f,
            glm::vec3(0.37f, 0.77, 0.29f)
        );

        //Create mesh and model
        Core::Mesh mesh;
        mesh.CreatePlane(20, 1.0f);
        _testModel = Core::MakeScope<Core::Model>
        (
            _resourceManager->GetTexture("BlockTexture"),
            _resourceManager->GetShader("ModelShader"),
            glm::vec3(1.0f, 1.0f, 1.0f),
            &mesh
        );

        _testGLTF = Core::MakeScope<Core::Model>
        (
            _resourceManager->GetTexture("SwordTexture"),
            _resourceManager->GetShader("ModelShader"),
            glm::vec3(1.0f, 1.0f, 1.0f),
            "../res/models/sword/scene.gltf"
        );
    }

    bool App::IsRunning()
    {
        return _windowManager->WindowIsRunning();
    }

    void App::Update()
    {
        //Process events
        {
            Core::PROFILE_SCOPE("Process events");
            _windowManager->PollEvents();
            _windowManager->ProcessEvents();
            InputManager::ProcessInput();
        }

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
            _renderer->Submit(_testModel.get());
            _renderer->Submit(_testGLTF.get());
            _renderer->Flush();
        }

        //Render UI (always after graphics)
        {
            Core::PROFILE_SCOPE("Render UI");
            _userInterface->AddElements();
            _userInterface->Render();
        }

        //End frame
        {
            Core::PROFILE_SCOPE("End frame");
            _windowManager->SwapBuffers();
        }
    }
}