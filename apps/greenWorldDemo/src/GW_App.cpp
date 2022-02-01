#include "GW_App.hpp"

namespace GW
{
    // ----- Private -----

    void App::LoadResources()
    {
        //Shader
        Core::ResourceManager::LoadShader("ShadowCreateShader", "../res/shader/greenWorld/shadowCreate_vs.glsl", "../res/shader/greenWorld/shadowCreate_fs.glsl");
        Core::ResourceManager::LoadShader("WaterPlaneShader", "../res/shader/greenWorld/waterPlane_vs.glsl", "../res/shader/greenWorld/waterPlane_fs.glsl");
        Core::ResourceManager::LoadShader("ModelShader", "../res/shader/greenWorld/model_vs.glsl", "../res/shader/greenWorld/model_fs.glsl");
        Core::ResourceManager::LoadShader("TerrainShader", "../res/shader/greenWorld/terrain_vs.glsl", "../res/shader/greenWorld/terrain_fs.glsl");
        Core::ResourceManager::LoadShader("CubemapShader", "../res/shader/greenWorld/cubemap_vs.glsl", "../res/shader/greenWorld/cubemap_fs.glsl");
        Core::ResourceManager::LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");
        Core::ResourceManager::LoadShader("SpriteShaderBW", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/spriteBlackAndWhite_fs.glsl");

        //Textures
        Core::ResourceManager::LoadTextureFromFile("ColorMap", "../res/textures/greenWorld/ColorMap.png");
        Core::ResourceManager::LoadTextureFromFile("DuDvMap", "../res/textures/greenWorld/DuDvMap.png");
    }

    void App::InitModules()
    {
        //Logging
        Core::Logger::Init();

        //Window
        _window = Core::MakeScope<Core::Window>();
        _window->SetTitle("GreenWorld Demo Application");

        //Camera
        _camera = Core::MakeScope<Core::Camera>(glm::vec3(-118.0f, 124.0f, 71.0f), 1.0f, -36.0f, 25.0f);

        //UI
        _interface = Core::MakeScope<Interface>(_window.get(), _camera.get());

        //Audio
        _audio = Core::MakeScope<Core::Audio>();
        _audio->SetListenerPosition(_camera->GetPosition(), _camera->GetFront(), _camera->GetUp());
        //_audio->PlaySound2D("../res/audio/greenWorld/music/TrueBlueSky.wav", true, 1.0f);
        //_audio->PlaySound3D("../res/audio/greenWorld/sounds/River.wav", glm::vec3(39.0f, 14.0f, 56.0f), true, 40.0f, 1.5);

        //Resources
        LoadResources();

        //Shadow-Rendering
        _shadowRenderer = Core::MakeScope<Core::ShadowRenderer>(8192, 8192, glm::vec3(150.0f, 100.0f, -30.0f));

        //Water-Rendering
        _waterRenderer = Core::MakeScope<Core::WaterRenderer>();

        //Model-Management
        Core::ModelManager::Init(_shadowRenderer.get());

        //Renderer (static)
        Core::Renderer::Init(_camera.get(), _shadowRenderer->GetLightProjection());

        //Input (static)
        InputManager::Init(_window.get(), _camera.get());
    }

    void App::CreateModels()
    {
        //Terrain
        auto terrainModel = Core::ModelManager::AddTerrain
        (
            PLANE_SIZE,
            PLANE_SIZE,
            1.0f,
            "GrassTexture",
            "../res/textures/greenWorld/Grass.jpg",
            "../res/textures/greenWorld/heightmap/Heightmap128_Noise.bmp"
        );
        terrainModel->ChangePosition(glm::vec3(0.0f, -2.7f, 0.0f));
        terrainModel->SetTexture2(Core::ResourceManager::GetTexture("ColorMap"));
        Core::Renderer::SubmitTerrain(terrainModel);

        //Water
        auto waterModel = Core::ModelManager::AddPlaneWithoutTexture
        (
            PLANE_SIZE - 112,
            PLANE_SIZE,
            1.0f
        );
        waterModel->ChangePosition(glm::vec3(30.5f, 0.0f, 0.0f));
        waterModel->SetTexture1(_waterRenderer->GetReflectTexture());
        waterModel->SetTexture2(_waterRenderer->GetRefractTexture());
        waterModel->SetTexture3(Core::ResourceManager::GetTexture("DuDvMap"));
        Core::Renderer::SubmitWater(waterModel);

        //House
        auto house = Core::ModelManager::AddObject("OldHouse", "../res/models/greenWorld/OldHouse");
        for(const auto& model : house)
        {
            model->ChangeSize(0.15f);
            model->ChangeRotation(0.0f, -90.0f, 0.0f);
            model->ChangePosition(glm::vec3(105.0f, 0.4f, 85.0f));
            Core::Renderer::Submit(model, true);
        }

        //Bridge
        auto bridge = Core::ModelManager::AddObject("Bridge", "../res/models/greenWorld/Bridge");
        for(const auto& model : bridge)
        {
            model->ChangeSize(2.0f);
            model->ChangeRotation(0.0f, -90.0f, 0.0f);
            model->ChangePosition(glm::vec3(39.0f, -0.45f, 42.0f));
            Core::Renderer::Submit(model, true);
        }

        //Tree
        auto tree = Core::ModelManager::AddObject("Tree", "../res/models/greenWorld/Tree");
        for(const auto& model : tree)
        {
            model->ChangeSize(0.06f);
            model->ChangePosition(glm::vec3(85.0f, 0.3f, 35.0f));
            Core::Renderer::Submit(model, true);
        }

        //Well
        auto well = Core::ModelManager::AddObject("Well", "../res/models/greenWorld/Well");
        for(const auto& model : well)
        {
            model->ChangeSize(0.05f);
            model->ChangeRotation(0.0f, -45.0f, 0.0f);
            model->ChangePosition(glm::vec3(75.0f, 15.3f, 75.0f));
            Core::Renderer::Submit(model, true);
        }
    }

    void App::CreateCubemap()
    {
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
        //Shadowsprite
        _shadowSprite = Core::MakeScope<Core::Sprite>
        (
            _shadowRenderer->GetDepthTexture(),
            Core::ResourceManager::GetShader("SpriteShaderBW"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _shadowSprite->ChangePosition(glm::vec2(10.0f, 10.0f));
        _shadowSprite->ChangeSize(glm::vec2(280.0f, 280.0f));
        Core::Renderer::Submit(_shadowSprite.get());

        //Reflectsprite
        _reflectSprite = Core::MakeScope<Core::Sprite>
        (
            _waterRenderer->GetReflectTexture(),
            Core::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _reflectSprite->ChangePosition(glm::vec2(300.0f, 10.0f));
        _reflectSprite->ChangeSize(glm::vec2(280.0f, 280.0f));
        Core::Renderer::Submit(_reflectSprite.get());

        //Refractsprite
        _refractSprite = Core::MakeScope<Core::Sprite>
        (
            _waterRenderer->GetRefractTexture(),
            Core::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _refractSprite->ChangePosition(glm::vec2(590.0f, 10.0f));
        _refractSprite->ChangeSize(glm::vec2(280.0f, 280.0f));
        Core::Renderer::Submit(_refractSprite.get());
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

            _window->CalcFrametime();
            _interface->PrepareFrame();
            Core::Renderer::PrepareFrame();
            Core::Renderer::ClearBuffers();
        }

        {   Core::PROFILE_SCOPE("Create shadows");

            _shadowRenderer->RenderToFramebuffer(Core::ResourceManager::GetShader("ShadowCreateShader"));
        }

        {   Core::PROFILE_SCOPE("Create water");

            _waterRenderer->RenderToFramebuffer(Core::ResourceManager::GetShader("TerrainShader"), Core::ResourceManager::GetShader("ModelShader"));
        }

        {   Core::PROFILE_SCOPE("Render graphics");

            Core::Renderer::FlushTerrainModel(Core::ResourceManager::GetShader("TerrainShader"));
            Core::Renderer::FlushAllModelBuffers(Core::ResourceManager::GetShader("ModelShader"));

            //Modify movefactor and render waterPlane
            _moveFactor += _waveSpeed * (float)_window->GetDeltaTime();
            _moveFactor  = fmod(_moveFactor, 1.0f);
            Core::Renderer::FlushWaterModel(Core::ResourceManager::GetShader("WaterPlaneShader"), _moveFactor);

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