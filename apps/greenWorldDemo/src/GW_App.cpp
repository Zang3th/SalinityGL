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
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%s:%g} [%level] %msg");
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
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-63.0f, 35.0f, 70.0f), -23.0f, -16.0f, 25.0f);
        _renderer = Core::MakeScope<Core::Renderer>(_camera.get());

        //Input & UI
        InputManager::Init(_windowManager.get(), _camera.get());
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get(), _camera.get());

        //Audio
        _audioManager = Core::MakeScope<Core::AudioManager>();
        _audioManager->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
        _audioManager->PlaySound2D("../res/audio/greenWorld/music/TrueBlueSky.wav", true, 1.0f);
        _audioManager->PlaySound3D("../res/audio/greenWorld/sounds/River.wav", glm::vec3(30.0f, 2.0f, 26.0f), true, 20.0f, 1.5);
    }

    void App::LoadResources()
    {
        //Textures
        Core::ResourceManager::LoadTexture("DirtTexture", "../res/textures/greenWorld/Dirt.jpg");
        Core::ResourceManager::LoadTexture("GrassTexture", "../res/textures/greenWorld/Grass.jpg");
        Core::ResourceManager::LoadTexture("WaterTexture", "../res/textures/greenWorld/Water.jpg");

        //Shaders
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
        Core::ResourceManager::LoadShader("CubemapShader", "../res/shader/greenWorld/cubemap_vs.glsl", "../res/shader/greenWorld/cubemap_fs.glsl");
    }

    void App::CreateModels()
    {
        //Create meshes
        Core::Mesh terrainMesh, waterMesh;

        //Create heightmap
        Core::Heightmap heightmap("../res/textures/greenWorld/heightmap/Heightmap64.bmp");

        //Fill meshes with data
        Core::MeshCreator::CreatePlane(PLANE_SIZE - 1, PLANE_SIZE - 1, 1.0f, &terrainMesh, &heightmap);
        Core::MeshCreator::CreatePlane(PLANE_SIZE - 1, PLANE_SIZE - 40, 1.0f, &waterMesh);

        //Add textures to meshes
        terrainMesh.diffuseTexture = Core::ResourceManager::GetTexture("GrassTexture");
        waterMesh.diffuseTexture = Core::ResourceManager::GetTexture("WaterTexture");

        //Create models out of meshes
        Core::Model terrainModel(&terrainMesh, Core::ResourceManager::GetShader("ModelShader"));
        Core::Model waterModel(&waterMesh, Core::ResourceManager::GetShader("ModelShader"));

        //Translate, rotate and scale models
        waterModel.IncreasePosition(glm::vec3(0.0f, 14.0f, 15.0f));

        //Save models in model vector
        _models.push_back(terrainModel);
        _models.push_back(waterModel);

        //Test the obj-Loader
        Core::Mesh obj;
        Core::MeshCreator::CreateFromObj("farmhouse_obj", "../res/models/greenWorld/Farmhouse", &obj);
        Core::Model objModel(&obj, Core::ResourceManager::GetShader("ModelShader"));
        _models.push_back(objModel);

    }

    void App::CreateCubemap()
    {
        std::array<const std::string, 6> faces
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
            _audioManager->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
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