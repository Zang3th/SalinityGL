#include "GreenWorldApp.hpp"

namespace GW
{
    // ----- Private -----

    void GreenWorldApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("ShadowCreateShader", "../Res/Shader/GreenWorld/ShadowCreate_VS.glsl", "../Res/Shader/GreenWorld/ShadowCreate_FS.glsl");
        Engine::ResourceManager::LoadShader("WaterShader", "../Res/Shader/GreenWorld/WaterPlane_VS.glsl", "../Res/Shader/GreenWorld/WaterPlane_FS.glsl");
        Engine::ResourceManager::LoadShader("ModelShader", "../Res/Shader/GreenWorld/Model_VS.glsl", "../Res/Shader/GreenWorld/Model_FS.glsl");
        Engine::ResourceManager::LoadShader("TerrainShader", "../Res/Shader/GreenWorld/Terrain_VS.glsl", "../Res/Shader/GreenWorld/Terrain_FS.glsl");
        Engine::ResourceManager::LoadShader("ParticleShader", "../Res/Shader/GreenWorld/Particle_VS.glsl", "../Res/Shader/GreenWorld/Particle_FS.glsl");
        Engine::ResourceManager::LoadShader("CubemapShader", "../Res/Shader/GreenWorld/Cubemap_VS.glsl", "../Res/Shader/GreenWorld/Cubemap_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShader", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/Sprite_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShaderGreyscale", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/SpriteGreyscale_FS.glsl");

        //Textures
        Engine::ResourceManager::LoadGLTextureAtlas("ParticleTextureAtlas", "../Res/Assets/Textures/GreenWorld/SmokeAtlas.png", 8);
        Engine::ResourceManager::LoadGLTexture("TerrainGrassTexture", "../Res/Assets/Textures/GreenWorld/Grass.jpg");
        Engine::ResourceManager::LoadGLTexture("TerrainColormap", "../Res/Assets/Textures/GreenWorld/Colormap128.png");
        Engine::ResourceManager::LoadGLTexture("WaterDuDvMap", "../Res/Assets/Textures/GreenWorld/DuDvMap.png");
        Engine::ResourceManager::LoadGLTexture("WaterNormalMap", "../Res/Assets/Textures/GreenWorld/WaterNormalMap.png");

        //Heightmap
        Engine::ResourceManager::LoadHeightmap("TerrainHeightmap", "../Res/Assets/Textures/GreenWorld/Heightmap128.bmp");
    }

    bool GreenWorldApp::Init()
    {
        Engine::CameraParams::startPitch = -24.0f;
        Engine::CameraParams::startPos   = glm::vec3(-75.0f, 74.0f, 70.0f);
        Engine::LightParams::position    = glm::vec3(150.0f, 100.0f, -30.0f);
        Engine::LightParams::target      = glm::vec3(64.0f, 0.0f, 64.0f);

        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("GreenWorld") != EXIT_SUCCESS)
        {
            return false;
        }
        Engine::Camera3D::Init();
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init3D();

        //Load shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer = new Engine::SceneRenderer();
        _sceneRenderer->SetTerrainShader("TerrainShader");
        _sceneRenderer->SetModelShader("ModelShader");
        _sceneRenderer->SetWaterShader("WaterShader");
        Engine::RenderManager::Submit(_sceneRenderer);

        _shadowRenderer = new Engine::ShadowRenderer(8192, 8192, glm::ortho(-90.0f, 90.0f, -90.0f, 90.0f, 110.0f, 210.0f), "ShadowCreateShader");
        _sceneRenderer->AddLightProjection(_shadowRenderer->GetLightProjection());
        Engine::RenderManager::Submit(_shadowRenderer);

        _waterRenderer  = new Engine::WaterRenderer();
        Engine::RenderManager::Submit(_waterRenderer);

        _spriteRenderer = new Engine::SpriteRenderer();
        Engine::RenderManager::Submit(_spriteRenderer);

        _particleRenderer = new Engine::ParticleRenderer
        (
            200,
            5.0f,
            0.05f,
            0.0f,
            6.0f,
            44.0f,
            "ParticleTextureAtlas",
            "ParticleShader",
            glm::vec3(87.0f, 34.0f, 92.5f)
        );
        Engine::RenderManager::Submit(_particleRenderer);

        //Create UI
        _interface = Engine::MakeScope<GreenWorldInterface>();
        _interface->Init();

        //Init audio system
        _audio = Engine::MakeScope<Engine::Audio>();
        if(_audio->GetInitStatus() == EXIT_SUCCESS)
        {
            _audio->PlaySound2D("../Res/Assets/Audio/GreenWorld/Music/TrueBlueSky.wav", true, 0.2f);
            _audio->PlaySound3D("../Res/Assets/Audio/GreenWorld/Sounds/River.wav", true, 4.0f, glm::vec3(39.0f, 14.0f, 56.0f), 0.5f);
        }

        AddObjects();
        AddSprites();

        return true;
    }

    void GreenWorldApp::AddObjects()
    {
        //Cubemap
        const std::array<const char*, 6> faces
        {
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_xp.jpg",   //Right
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_xn.jpg",   //Left
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_yp.jpg",   //Top
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_yn.jpg",   //Bottom
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_zp.jpg",   //Front
            "../Res/Assets/Textures/GreenWorld/Cubemap/Graycloud_zn.jpg"    //Back
        };
        _sceneRenderer->AddCubemap(faces, "CubemapShader");

        //Terrain
        _sceneRenderer->AddTerrain
        (
            Engine::RenderParams::planeSize,
            Engine::RenderParams::planeSize,
            1.0f,
            glm::vec3(0.0f, -2.7f, 0.0f),
            _shadowRenderer->GetDepthTexture(),
            "TerrainGrassTexture",
            "TerrainColormap",
            "TerrainHeightmap"
        );

        //Water
        _sceneRenderer->AddWater
        (
            Engine::RenderParams::planeSize - 112,
            Engine::RenderParams::planeSize,
            1.0f,
            glm::vec3(30.5f, 0.0f, 0.0f),
            0.025f,
            "WaterDuDvMap",
            "WaterNormalMap",
            _waterRenderer->GetReflectTexture(),
            _waterRenderer->GetRefractTexture(),
            _waterRenderer->GetRefractDepthTexture()
        );

        //House
        _sceneRenderer->AddObject
        (
            1.2f,
            glm::vec3(0.0f, -70.0f, 0.0f),
            glm::vec3(85.0f, 0.45f, 95.0f),
            _shadowRenderer->GetDepthTexture(),
            "House",
            "../Res/Assets/Models/GreenWorld/House"
        );

        //Bridge
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(39.0f, -0.45f, 47.0f),
            _shadowRenderer->GetDepthTexture(),
            "Bridge",
            "../Res/Assets/Models/GreenWorld/Bridge"
        );

        //Tree
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(85.0f, 0.3f, 20.0f),
            _shadowRenderer->GetDepthTexture(),
            "Tree",
            "../Res/Assets/Models/GreenWorld/Tree"
        );
    }

    void GreenWorldApp::AddSprites()
    {
        //Shadow sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(210.0f, 0.0f),
            _shadowRenderer->GetDepthTexture(),
            Engine::ResourceManager::GetShader("SpriteShaderGreyscale")
        );

        //Reflect sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(410.0f, 0.0f),
            _waterRenderer->GetReflectTexture(),
            Engine::ResourceManager::GetShader("SpriteShader")
        );

        //Refract sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(610.0f, 0.0f),
            _waterRenderer->GetRefractTexture(),
            Engine::ResourceManager::GetShader("SpriteShader")
        );

        //Refract depth sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(810.0f, 0.0f),
            _waterRenderer->GetRefractDepthTexture(),
            Engine::ResourceManager::GetShader("SpriteShaderGreyscale")
        );
    }

    // ----- Public -----

    GreenWorldApp::GreenWorldApp() = default;

    GreenWorldApp::~GreenWorldApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
        Engine::Window::Close();
    }

    void GreenWorldApp::Update()
    {
        {
            Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
            Engine::CameraController3D::ProcessInput();
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
            _interface->PrepareFrame();
            _audio->SetListenerPosition(Engine::Camera3D::GetPosition(), Engine::Camera3D::GetFront(), Engine::Camera3D::GetUp());
        }

        {
            Engine::PROFILE_SCOPE("Render shadows");

            _shadowRenderer->Flush((Engine::Renderer*)_sceneRenderer);
        }

        {
            Engine::PROFILE_SCOPE("Render water");

            _waterRenderer->Flush((Engine::Renderer*)_sceneRenderer);
        }

        {
            Engine::PROFILE_SCOPE("Render scene");

            _sceneRenderer->Flush(nullptr);
            _particleRenderer->Flush((Engine::Renderer*)_sceneRenderer);

            if(Engine::UIParams::debugSprites)
            {
                _spriteRenderer->Flush(nullptr);
            }
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
