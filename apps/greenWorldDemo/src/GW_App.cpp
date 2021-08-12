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
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-55.0f, 58.0f, 29.0f), 0.0f, -37.0f, 15.0f);
        _renderer = Core::MakeScope<Core::Renderer>(_camera.get());

        //Input & UI
        InputManager::Init(_windowManager.get(), _camera.get());
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get(), _camera.get());
    }

    void App::LoadResources()
    {
        //Textures
        Core::ResourceManager::LoadTexture("BlockTexture", "../res/textures/greenWorld/Block.jpg");
        Core::ResourceManager::LoadTexture("SwordTexture", "../res/models/greenWorld/sword/textures/Object001_mtl_baseColor.jpeg");

        //Shaders
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
        Core::ResourceManager::LoadShader("CubemapShader", "../res/shader/greenWorld/cubemap_vs.glsl", "../res/shader/greenWorld/cubemap_fs.glsl");
    }

    void App::CreateModels()
    {
        //Create meshes
        Core::Mesh plane, sword;

        //Fill meshes with data
        Core::MeshCreator::CreatePlane(20, 1.0f, &plane);
        Core::MeshCreator::CreateFromGLTF("../res/models/greenWorld/sword/scene.gltf", &sword);

        //Create models out of meshes
        _models.emplace_back(Core::Model(Core::ResourceManager::GetTexture("BlockTexture"), Core::ResourceManager::GetShader("ModelShader"), &plane));
        _models.emplace_back(Core::Model(Core::ResourceManager::GetTexture("SwordTexture"), Core::ResourceManager::GetShader("ModelShader"), &sword));
    }

    void App::CreateCubemap()
    {
        std::vector<const char*> faces
        {
            "../res/textures/greenWorld/cubemap/graycloud_xp.jpg", //Right
            "../res/textures/greenWorld/cubemap/graycloud_xn.jpg", //Left
            "../res/textures/greenWorld/cubemap/graycloud_yp.jpg", //Top
            "../res/textures/greenWorld/cubemap/graycloud_yn.jpg", //Bottom
            "../res/textures/greenWorld/cubemap/graycloud_zp.jpg", //Front
            "../res/textures/greenWorld/cubemap/graycloud_zn.jpg"  //Back
        };

        _cubemap = Core::MakeScope<Core::Cubemap>(faces, Core::ResourceManager::GetShader("CubemapShader"));
    }

    // ----- Public -----

    App::App()
    {
        ConfigureLogger();
        ConfigureProfiler();
        InitModules();

        //Call after init because these methods depend on OpenGL-Initialization
        LoadResources();
        CreateModels();
        CreateCubemap();
    }

    App::~App()
    {
        Core::ResourceManager::CleanUp();
    }

    bool App::IsRunning()
    {
        return _windowManager->WindowIsRunning();
    }

    void App::Update()
    {
        {   Core::PROFILE_SCOPE("Process events");

            _windowManager->PollEvents();
            _windowManager->ProcessEvents();
            InputManager::ProcessInput();
        }

        {   Core::PROFILE_SCOPE("Prepare frame");

            _windowManager->PrepareFrame();
            _renderer->Prepare();
            _userInterface->PrepareFrame();
        }

        {   Core::PROFILE_SCOPE("Render graphics");

            for(const auto& model : _models)
                _renderer->Submit(&model);

            _renderer->Submit(_cubemap.get());
            _renderer->Flush();
        }

        {   Core::PROFILE_SCOPE("Render UI");

            _userInterface->AddElements();
            _userInterface->Render();
        }

        {   Core::PROFILE_SCOPE("End frame");

            _windowManager->SwapBuffers();
        }
    }
}