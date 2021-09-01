#include "GW_App.hpp"

namespace GW
{
    // ----- Private -----

    void App::InitModules()
    {
        //Logging
        Core::Logger::Init();

        //Window
        _windowManager = Core::MakeScope<Core::WindowManager>();
        _windowManager->SetWindowTitle("GreenWorld Demo Application");

        //Camera & Renderer
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-29.0f, 45.0f, 20.0f), 24.0f, -15.0f, 35.0f);
        _renderer = Core::MakeScope<Core::Renderer>(_camera.get());

        //Input & UI
        InputManager::Init(_windowManager.get(), _camera.get());
        _userInterface = Core::MakeScope<Interface>(_windowManager.get(), _renderer.get(), _camera.get());

        //Audio
        _audioManager = Core::MakeScope<Core::AudioManager>();
        _audioManager->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
        _audioManager->PlaySound2D("../res/audio/greenWorld/music/TrueBlueSky.wav", true, 1.0f);
        _audioManager->PlaySound3D("../res/audio/greenWorld/sounds/River.wav", glm::vec3(39.0f, 14.0f, 56.0f), true, 40.0f, 1.5);

        //Shadow-Rendering
        Core::ResourceManager::LoadShader("ShadowShader", "../res/shader/greenWorld/shadow_vs.glsl", "../res/shader/greenWorld/shadow_fs.glsl");
        _shadowRenderer = Core::MakeScope<Core::ShadowRenderer>(Core::ResourceManager::GetShader("ShadowShader"), 2048, 2048, glm::vec3(140.0f, 60.0f, 60.0f));
    }

    void App::CreateModels()
    {
        //Textures
        Core::ResourceManager::LoadTexture("GrassTexture", "../res/textures/greenWorld/Grass.jpg");
        Core::ResourceManager::LoadTexture("WaterTexture", "../res/textures/greenWorld/Water.jpg");

        //Shader
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");

        //Create terrain
        {
            Core::Mesh terrainMesh;
            Core::Heightmap heightmap("../res/textures/greenWorld/heightmap/Heightmap128.bmp");
            Core::MeshCreator::CreatePlane(PLANE_SIZE - 1, PLANE_SIZE - 1, 1.0f, &terrainMesh, &heightmap);
            terrainMesh.diffuseTexture = Core::ResourceManager::GetTexture("GrassTexture");
            Core::Model terrainModel(&terrainMesh);
            _models.push_back(terrainModel);
        }

        //Create water
        {
            Core::Mesh waterMesh;
            Core::MeshCreator::CreatePlane(PLANE_SIZE - 100, PLANE_SIZE - 1, 1.0f, &waterMesh);
            waterMesh.diffuseTexture = Core::ResourceManager::GetTexture("WaterTexture");
            Core::Model waterModel(&waterMesh);
            waterModel.ChangePosition(glm::vec3(25.0f, 14.0f, 0.0f));
            _models.push_back(waterModel);
        }

        //Create house
        {
            std::vector<Core::Mesh> meshes;
            Core::MeshCreator::CreateFromObj("OldHouse", "../res/models/greenWorld/OldHouse", &meshes);
            for(auto& mesh : meshes)
            {
                Core::Model meshModel(&mesh);
                meshModel.ChangeSize(0.15f);
                meshModel.ChangeRotation(0.0f, -90.0f, 0.0f);
                meshModel.ChangePosition(glm::vec3(105.0f, 15.5f, 85.0f));
                _models.push_back(meshModel);
            }
        }

        //Create bridge
        {
            std::vector<Core::Mesh> meshes;
            Core::MeshCreator::CreateFromObj("Bridge", "../res/models/greenWorld/Bridge", &meshes);
            for(auto& mesh : meshes)
            {
                Core::Model meshModel(&mesh);
                meshModel.ChangeSize(2.0f);
                meshModel.ChangeRotation(0.0f, -90.0f, 0.0f);
                meshModel.ChangePosition(glm::vec3(38.0f, 15.0f, 40.0f));
                _models.push_back(meshModel);
            }
        }

        //Create tree
        {
            std::vector<Core::Mesh> meshes;
            Core::MeshCreator::CreateFromObj("Tree", "../res/models/greenWorld/Tree", &meshes);
            for(auto& mesh : meshes)
            {
                Core::Model meshModel(&mesh);
                meshModel.ChangeSize(0.07f);
                meshModel.ChangePosition(glm::vec3(100.0f, 16.0f, 28.0f));
                _models.push_back(meshModel);
            }
        }

        //Create well
        {
            std::vector<Core::Mesh> meshes;
            Core::MeshCreator::CreateFromObj("Well", "../res/models/greenWorld/Well", &meshes);
            for(auto& mesh : meshes)
            {
                Core::Model meshModel(&mesh);
                meshModel.ChangeSize(0.05f);
                meshModel.ChangeRotation(0.0f, -45.0f, 0.0f);
                meshModel.ChangePosition(glm::vec3(75.0f, 30.0f, 75.0f));
                _models.push_back(meshModel);
            }
        }

        //Submit models
        for(const auto& model : _models)
            _renderer->Submit(&model);
    }

    void App::CreateCubemap()
    {
        Core::ResourceManager::LoadShader("CubemapShader", "../res/shader/greenWorld/cubemap_vs.glsl", "../res/shader/greenWorld/cubemap_fs.glsl");

        const std::array<const char*, 6> faces
        {
            "../res/textures/greenWorld/cubemap/graycloud_xp.jpg", //Right
            "../res/textures/greenWorld/cubemap/graycloud_xn.jpg", //Left
            "../res/textures/greenWorld/cubemap/graycloud_yp.jpg", //Top
            "../res/textures/greenWorld/cubemap/graycloud_yn.jpg", //Bottom
            "../res/textures/greenWorld/cubemap/graycloud_zp.jpg", //Front
            "../res/textures/greenWorld/cubemap/graycloud_zn.jpg"  //Back
        };

        _cubemap = Core::MakeScope<Core::Cubemap>(faces, Core::ResourceManager::GetShader("CubemapShader"));
        _renderer->Submit(_cubemap.get());
    }

    void App::CreateSprites()
    {
        Core::ResourceManager::LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");

        _testSprite = Core::MakeScope<Core::Sprite>
        (
            Core::ResourceManager::GetTexture("WaterTexture"),
            Core::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _testSprite->ChangePosition(glm::vec2(1350.0f, 50.0f));
        _testSprite->ChangeSize(glm::vec2(512.0f, 512.0f));

        _renderer->Submit(_testSprite.get());
    }

    // ----- Public -----

    App::App()
    {
        //ConfigureProfiler();
        InitModules();

        //Call after init because these methods depend on OpenGL-Initialization
        CreateModels();
        CreateCubemap();
        CreateSprites();
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

            _windowManager->Prepare();
            _windowManager->CalcFrametime();
            _renderer->PrepareFrame();
            _userInterface->PrepareFrame();
        }

        {   Core::PROFILE_SCOPE("Create shadows");

            // Render scene to shadow framebuffer
            _shadowRenderer->StartFrame();
            _renderer->FlushModels(_shadowRenderer->GetShader());
            _shadowRenderer->EndFrame();

            // Clear buffers
            _windowManager->Prepare();
        }

        {   Core::PROFILE_SCOPE("Render graphics");

            _testSprite->SetTexture(_shadowRenderer->GetDepthTexture());
            _renderer->FlushEverything(Core::ResourceManager::GetShader("ModelShader"));
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