#include "GW_App.hpp"

namespace GW
{
    // ----- Private -----

    void App::InitModules()
    {
        //Logging
        Core::Logger::Init();

        //Window
        _window = Core::MakeScope<Core::Window>();
        _window->SetTitle("GreenWorld Demo Application");

        //Camera
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-101.0f, 92.0f, 32.0f), 16.0f, -31.0f, 25.0f);

        //Renderer (static)
        Core::Renderer::Init(_camera.get());

        //Input (static)
        InputManager::Init(_window.get(), _camera.get());

        //UI
        _interface = Core::MakeScope<Interface>(_window.get(), _camera.get());

        //Audio
        _audio = Core::MakeScope<Core::Audio>();
        _audio->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
        _audio->PlaySound2D("../res/audio/greenWorld/music/TrueBlueSky.wav", true, 1.0f);
        _audio->PlaySound3D("../res/audio/greenWorld/sounds/River.wav", glm::vec3(39.0f, 14.0f, 56.0f), true, 40.0f, 1.5);

        //Shadow-Rendering
        Core::ResourceManager::LoadShader("ShadowShader", "../res/shader/greenWorld/shadow_vs.glsl", "../res/shader/greenWorld/shadow_fs.glsl");
        _shadowRenderer = Core::MakeScope<Core::ShadowRenderer>(2048, 2048, glm::vec3(143.0f, 175.0f, -15.0f));

        //Model-Management
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
        Core::ModelManager::Init(_shadowRenderer.get());
    }

    void App::CreateModels()
    {
        //Terrain
        Core::Renderer::Submit
        (
            Core::ModelManager::AddTerrain
            (
                PLANE_SIZE,
                PLANE_SIZE,
                1.0f,
                "GrassTexture",
                "../res/textures/greenWorld/Grass.jpg",
                "../res/textures/greenWorld/heightmap/Heightmap128.bmp"
            )
        );

        //Water
        auto water = Core::ModelManager::AddPlane
        (
            PLANE_SIZE - 100,
            PLANE_SIZE,
            1.0f,
            "WaterTexture",
            "../res/textures/greenWorld/Water.jpg"
        );
        water->ChangePosition(glm::vec3(25.0f, 2.8f, 0.0f));
        Core::Renderer::Submit(water);

        //Ground
        auto ground = Core::ModelManager::AddObject("Pyramid", "../res/models/greenWorld/Pyramid");
        for(const auto& model : ground)
        {
            model->ChangeSize(12.7f);
            model->ChangeRotation(180.0f, 0.0f, 0.0f);
            model->ChangePosition(glm::vec3(128.0f, 2.5f, 1.0f));
            Core::Renderer::Submit(model);
        }

        //House
        auto house = Core::ModelManager::AddObject("OldHouse", "../res/models/greenWorld/OldHouse");
        for(const auto& model : house)
        {
            model->ChangeSize(0.15f);
            model->ChangeRotation(0.0f, -90.0f, 0.0f);
            model->ChangePosition(glm::vec3(105.0f, 3.0f, 85.0f));
            Core::Renderer::Submit(model);
        }

        //Bridge
        auto bridge = Core::ModelManager::AddObject("Bridge", "../res/models/greenWorld/Bridge");
        for(const auto& model : bridge)
        {
            model->ChangeSize(2.0f);
            model->ChangeRotation(0.0f, -90.0f, 0.0f);
            model->ChangePosition(glm::vec3(38.0f, 3.0f, 40.0f));
            Core::Renderer::Submit(model);
        }

        //Tree
        auto tree = Core::ModelManager::AddObject("Tree", "../res/models/greenWorld/Tree");
        for(const auto& model : tree)
        {
            model->ChangeSize(0.07f);
            model->ChangePosition(glm::vec3(100.0f, 3.0f, 28.0f));
            Core::Renderer::Submit(model);
        }

        //Well
        auto well = Core::ModelManager::AddObject("Well", "../res/models/greenWorld/Well");
        for(const auto& model : well)
        {
            model->ChangeSize(0.05f);
            model->ChangeRotation(0.0f, -45.0f, 0.0f);
            model->ChangePosition(glm::vec3(75.0f, 18.0f, 75.0f));
            Core::Renderer::Submit(model);
        }
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
        Core::Renderer::Submit(_cubemap.get());
    }

    void App::CreateSprites()
    {
        Core::ResourceManager::LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");

        _testSprite = Core::MakeScope<Core::Sprite>
        (
            _shadowRenderer->GetDepthTexture(),
            Core::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _testSprite->ChangePosition(glm::vec2(10.0f, 10.0f));
        _testSprite->ChangeSize(glm::vec2(280.0f, 280.0f));

        Core::Renderer::Submit(_testSprite.get());
    }

    // ----- Public -----

    App::App()
    {
        InitModules();

        //Call after init because these methods depend on OpenGL-Initialization
        CreateModels();
        CreateCubemap();
        CreateSprites();
    }

    App::~App()
    {
        Core::ResourceManager::CleanUp();
        Core::ModelManager::CleanUp();
    }

    bool App::IsRunning()
    {
        return _window->IsRunning();
    }

    void App::Update()
    {
        {   Core::PROFILE_SCOPE("Process events");

            _window->PollEvents();
            _window->ProcessEvents();
            InputManager::ProcessInput();
            _audio->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
        }

        {   Core::PROFILE_SCOPE("Prepare frame");

            _window->ClearBuffers();
            _window->CalcFrametime();
            Core::Renderer::PrepareFrame();
            _interface->PrepareFrame();
        }

        {   Core::PROFILE_SCOPE("Create shadows");

            // Render scene to shadow framebuffer
            _shadowRenderer->StartFrame(Core::ResourceManager::GetShader("ShadowShader"));
            Core::Renderer::FlushModels(Core::ResourceManager::GetShader("ShadowShader"), _shadowRenderer->GetLightProjection());
            _shadowRenderer->EndFrame();

            // Clear buffers
            _window->ClearBuffers();
        }

        {   Core::PROFILE_SCOPE("Render graphics");

            Core::Renderer::FlushModels(Core::ResourceManager::GetShader("ModelShader"), _shadowRenderer->GetLightProjection());
            Core::Renderer::FlushSprites();
            Core::Renderer::FlushCubemap();
        }

        {   Core::PROFILE_SCOPE("Render UI");

            _interface->AddElements();
            _interface->Render();
        }

        {   Core::PROFILE_SCOPE("End frame");

            _window->SwapBuffers();
        }
    }
}