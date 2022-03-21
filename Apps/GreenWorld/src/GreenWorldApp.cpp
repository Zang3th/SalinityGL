#include "GreenWorldApp.hpp"

//Initialize extern settings
const Engine::uint32 Engine::WINDOW_WIDTH        = 1920;
const Engine::uint32 Engine::WINDOW_HEIGHT       = 1080;
const Engine::uint32 Engine::PLANE_SIZE          = 128;
      bool           Engine::WIREFRAME_RENDERING = false;
      bool           Engine::DEBUG_SPRITES       = false;
const float          Engine::GRAVITY             = -20.0f;

namespace GreenWorld
{
    // ----- Private -----

    void App::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("ShadowCreateShader", "../Res/Shader/GreenWorld/ShadowCreate_VS.glsl", "../Res/Shader/GreenWorld/ShadowCreate_FS.glsl");
        Engine::ResourceManager::LoadShader("WaterPlaneShader", "../Res/Shader/GreenWorld/WaterPlane_VS.glsl", "../Res/Shader/GreenWorld/WaterPlane_FS.glsl");
        Engine::ResourceManager::LoadShader("ModelShader", "../Res/Shader/GreenWorld/Model_VS.glsl", "../Res/Shader/GreenWorld/Model_FS.glsl");
        Engine::ResourceManager::LoadShader("TerrainShader", "../Res/Shader/GreenWorld/Terrain_VS.glsl", "../Res/Shader/GreenWorld/Terrain_FS.glsl");
        Engine::ResourceManager::LoadShader("ParticleShader", "../Res/Shader/GreenWorld/Particle_VS.glsl", "../Res/Shader/GreenWorld/Particle_FS.glsl");
        Engine::ResourceManager::LoadShader("CubemapShader", "../Res/Shader/GreenWorld/Cubemap_VS.glsl", "../Res/Shader/GreenWorld/Cubemap_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShader", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/Sprite_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShaderBW", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/SpriteBlackAndWhite_FS.glsl");

        //Textures
        Engine::ResourceManager::LoadTextureFromFile("ColorMap", "../Res/Assets/Textures/GreenWorld/Colormap128.png");
        Engine::ResourceManager::LoadTextureFromFile("DuDvMap", "../Res/Assets/Textures/GreenWorld/DuDvMap.png");
        Engine::ResourceManager::LoadTextureFromFile("NormalMap", "../Res/Assets/Textures/GreenWorld/WaterNormalMap.png");
        Engine::ResourceManager::LoadTextureAtlasFromFile("ParticleTextureAtlas", "../Res/Assets/Textures/GreenWorld/Star.png", 1);
    }

    void App::InitModules()
    {
        //Logging
        Engine::Logger::Init();

        //Window
        Engine::Window::Init("GreenWorld Demo Application");

        //Camera + Control
        Engine::Camera::Init(glm::vec3(-75.0f, 74.0f, 70.0f), 0.4f, -23.0f, 25.0f);
        Engine::CameraController3D::Init();

        //UI
        _interface = Engine::MakeScope<Interface>();

        //Audio
        _audio = Engine::MakeScope<Engine::Audio>();
        _audio->SetListenerPosition(Engine::Camera::GetPosition(), Engine::Camera::GetFront(), Engine::Camera::GetUp());
        //_audio->PlaySound2D("../Res/Assets/Audio/GreenWorld/Music/TrueBlueSky.wav", true, 1.0f);
        //_audio->PlaySound3D("../Res/Assets/Audio/GreenWorld/Sounds/River.wav", glm::vec3(39.0f, 14.0f, 56.0f), true, 40.0f, 1.5);

        //Resources
        LoadResources();

        //Shadow-Rendering
        _shadowRenderer = Engine::MakeScope<Engine::ShadowRenderer>(8192, 8192, _lightPosition);

        //Water-Rendering
        _waterRenderer = Engine::MakeScope<Engine::WaterRenderer>();

        //Renderer
        Engine::Renderer::Init(_nearPlane, _farPlane, _lightPosition, _lightColor, _shadowRenderer->GetLightProjection());
    }

    void App::CreateModels()
    {
        //Terrain
        auto terrainModel = Engine::ModelManager::AddTerrain
        (
            Engine::PLANE_SIZE,
            Engine::PLANE_SIZE,
            1.0f,
            "GrassTexture",
            "../Res/Assets/Textures/GreenWorld/Grass.jpg",
            "../Res/Assets/Textures/GreenWorld/Heightmap128.bmp"
        );
        terrainModel->ChangePosition(glm::vec3(0.0f, -2.7f, 0.0f));
        terrainModel->SetTexture2(Engine::ResourceManager::GetTexture("ColorMap"));
        terrainModel->SetTexture3(_shadowRenderer->GetDepthTexture());
        Engine::Renderer::SubmitTerrain(terrainModel);

        //Water
        auto waterModel = Engine::ModelManager::AddPlane
        (
            Engine::PLANE_SIZE - 112,
            Engine::PLANE_SIZE,
            1.0f
        );
        waterModel->ChangePosition(glm::vec3(30.5f, 0.0f, 0.0f));
        waterModel->SetTexture1(_waterRenderer->GetReflectTexture());
        waterModel->SetTexture2(_waterRenderer->GetRefractTexture());
        waterModel->SetTexture3(Engine::ResourceManager::GetTexture("DuDvMap"));
        waterModel->SetTexture4(Engine::ResourceManager::GetTexture("NormalMap"));
        waterModel->SetTexture5(_waterRenderer->GetRefractDepthTexture());
        Engine::Renderer::SubmitWater(waterModel);

        //House
        auto house = Engine::ModelManager::AddObject("House", "../Res/Assets/Models/GreenWorld/House");
        for(const auto& model : house)
        {
            model->SetTexture3(_shadowRenderer->GetDepthTexture());
            model->ChangeSize(1.2f);
            model->ChangeRotation(0.0f, -70.0f, 0.0f);
            model->ChangePosition(glm::vec3(85.0f, 0.45f, 95.0f));
            Engine::Renderer::Submit(model);
        }

        //Bridge
        auto bridge = Engine::ModelManager::AddObject("Bridge", "../Res/Assets/Models/GreenWorld/Bridge");
        for(const auto& model : bridge)
        {
            model->SetTexture3(_shadowRenderer->GetDepthTexture());
            model->ChangePosition(glm::vec3(39.0f, -0.45f, 47.0f));
            Engine::Renderer::Submit(model);
        }

        //Tree
        auto tree = Engine::ModelManager::AddObject("Tree", "../Res/Assets/Models/GreenWorld/Tree");
        for(const auto& model : tree)
        {
            model->SetTexture3(_shadowRenderer->GetDepthTexture());
            model->ChangePosition(glm::vec3(85.0f, 0.3f, 20.0f));
            Engine::Renderer::Submit(model);
        }
    }

    void App::CreateCubemap()
    {
        const std::array<const char*, 6> faces
        {
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_xp.jpg", //Right
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_xn.jpg", //Left
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_yp.jpg", //Top
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_yn.jpg", //Bottom
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_zp.jpg", //Front
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_zn.jpg"  //Back
        };

        _cubemap = Engine::MakeScope<Engine::Cubemap>(faces, Engine::ResourceManager::GetShader("CubemapShader"));
        Engine::Renderer::Submit(_cubemap.get());
    }

    void App::CreateSprites()
    {
        //ShadowSprite
        _shadowSprite = Engine::MakeScope<Engine::Sprite>
        (
            _shadowRenderer->GetDepthTexture(),
            Engine::ResourceManager::GetShader("SpriteShaderBW"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _shadowSprite->ChangePosition(glm::vec2(210.0f, 0));
        _shadowSprite->ChangeSize(glm::vec2(200.0f, 200.0f));
        Engine::Renderer::Submit(_shadowSprite.get());

        //ReflectSprite
        _reflectSprite = Engine::MakeScope<Engine::Sprite>
        (
            _waterRenderer->GetReflectTexture(),
            Engine::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _reflectSprite->ChangePosition(glm::vec2(410.0f, 0));
        _reflectSprite->ChangeSize(glm::vec2(200.0f, 200.0f));
        Engine::Renderer::Submit(_reflectSprite.get());

        //RefractSprite
        _refractSprite = Engine::MakeScope<Engine::Sprite>
        (
            _waterRenderer->GetRefractTexture(),
            Engine::ResourceManager::GetShader("SpriteShader"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _refractSprite->ChangePosition(glm::vec2(610.0f, 0));
        _refractSprite->ChangeSize(glm::vec2(200.0f, 200.0f));
        Engine::Renderer::Submit(_refractSprite.get());

        //RefractDepthSprite
        _refractDepthSprite = Engine::MakeScope<Engine::Sprite>
        (
            _waterRenderer->GetRefractDepthTexture(),
            Engine::ResourceManager::GetShader("SpriteShaderBW"),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        _refractDepthSprite->ChangePosition(glm::vec2(810.0f, 0));
        _refractDepthSprite->ChangeSize(glm::vec2(200.0f, 200.0f));
        Engine::Renderer::Submit(_refractDepthSprite.get());
    }

    void App::CreateParticles()
    {
        _particleRenderer = Engine::MakeScope<Engine::ParticleRenderer>
        (
            Engine::ResourceManager::GetTexture("ParticleTextureAtlas"),
            Engine::ResourceManager::GetShader("ParticleShader"),
            glm::vec3(0.0f, 0.0f, 0.0f),
            300,
            1.0f,
            1.0f,
            4.0f
        );
        Engine::Renderer::Submit(_particleRenderer.get());
    }

    // ----- Public -----

    App::App()
    {
        InitModules();

        //Call after init because these methods depend on OpenGL-Initialization
        CreateModels();
        CreateParticles();
        CreateCubemap();
        CreateSprites();
    }

    App::~App()
    {
        Engine::ResourceManager::CleanUp();
        Engine::ModelManager::CleanUp();
    }

    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }

    void App::Update()
    {
        {   Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
            Engine::CameraController3D::ProcessInput();
            _audio->SetListenerPosition(Engine::Camera::GetPosition(), Engine::Camera::GetFront(), Engine::Camera::GetUp());
        }

        {   Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            _interface->PrepareFrame();
            Engine::Renderer::PrepareFrame();
            Engine::Renderer::ClearBuffers();
        }

        {   Engine::PROFILE_SCOPE("Create shadows");

            _shadowRenderer->RenderToFramebuffer(Engine::ResourceManager::GetShader("ShadowCreateShader"));
        }

        {   Engine::PROFILE_SCOPE("Create water");

            _waterRenderer->RenderToFramebuffer(Engine::ResourceManager::GetShader("TerrainShader"), Engine::ResourceManager::GetShader("ModelShader"));
        }

        {   Engine::PROFILE_SCOPE("Render graphics");

            Engine::Renderer::FlushTerrain(Engine::ResourceManager::GetShader("TerrainShader"));
            Engine::Renderer::FlushModelBuffer(Engine::ResourceManager::GetShader("ModelShader"));

            //Modify movefactor and render waterPlane
            _moveFactor += _waveSpeed * (float)Engine::Window::GetDeltaTime();
            _moveFactor  = fmod(_moveFactor, 1.0f);
            Engine::Renderer::FlushWater(Engine::ResourceManager::GetShader("WaterPlaneShader"), _moveFactor);

            if(Engine::DEBUG_SPRITES)
                Engine::Renderer::FlushSprites();

            Engine::Renderer::FlushCubemap();

            Engine::Renderer::FlushParticleRenderer();
        }

        {   Engine::PROFILE_SCOPE("Render UI");

            _interface->AddElements();
            _interface->Render();
        }

        {   Engine::PROFILE_SCOPE("End frame");

            Engine::Window::SwapBuffers();
        }
    }
}