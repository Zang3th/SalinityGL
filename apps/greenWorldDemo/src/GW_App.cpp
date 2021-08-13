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
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-61.0f, 48.0f, 76.0f), -29.0f, -23.0f, 15.0f);
        _renderer = Core::MakeScope<Core::Renderer>(_camera.get());

        //Input & UI
        InputManager::Init(_windowManager.get(), _camera.get());
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get(), _camera.get());
    }

    void App::LoadResources()
    {
        //Textures
        Core::ResourceManager::LoadTexture("GrassTexture", "../res/textures/greenWorld/Grass.jpg");
        Core::ResourceManager::LoadTexture("WaterTexture", "../res/textures/greenWorld/Water.jpg");
        Core::ResourceManager::LoadTexture("BridgeTexture", "../res/models/greenWorld/bridge/textures/Material_baseColor.jpg");

        //Shaders
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
        Core::ResourceManager::LoadShader("CubemapShader", "../res/shader/greenWorld/cubemap_vs.glsl", "../res/shader/greenWorld/cubemap_fs.glsl");
    }

    void App::CreateModels()
    {
        //Create meshes
        Core::Mesh terrain, water, bridge;

        //Create heightmap
        Core::Heightmap heightmap("../res/textures/greenWorld/heightmap/Heightmap64.bmp");

        //Fill meshes with data
        Core::MeshCreator::CreateTerrain(PLANE_SIZE - 1, 1.0f, &terrain, &heightmap);
        Core::MeshCreator::CreateTerrain(PLANE_SIZE - 1, 1.0f, &water);
        Core::MeshCreator::CreateFromGLTF("../res/models/greenWorld/bridge/scene.gltf", &bridge);

        //Create models out of meshes
        _models.emplace_back(Core::Model(Core::ResourceManager::GetTexture("GrassTexture"), Core::ResourceManager::GetShader("ModelShader"), &terrain));
        _models.emplace_back(Core::Model(Core::ResourceManager::GetTexture("WaterTexture"), Core::ResourceManager::GetShader("ModelShader"), &water));
        _models.emplace_back(Core::Model(Core::ResourceManager::GetTexture("BridgeTexture"), Core::ResourceManager::GetShader("ModelShader"), &bridge));
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
            _renderer->PrepareFrame();
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