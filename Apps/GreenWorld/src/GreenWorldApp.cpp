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
        Engine::ResourceManager::LoadTextureAtlasFromFile("ParticleTextureAtlas", "../Res/Assets/Textures/GreenWorld/SmokeAtlas.png", 8);
        Engine::ResourceManager::LoadTextureFromFile("TerrainGrassTexture", "../Res/Assets/Textures/GreenWorld/Grass.jpg");
        Engine::ResourceManager::LoadTextureFromFile("TerrainColormap", "../Res/Assets/Textures/GreenWorld/Colormap128.png");
        Engine::ResourceManager::LoadTextureFromFile("WaterDuDvMap", "../Res/Assets/Textures/GreenWorld/DuDvMap.png");
        Engine::ResourceManager::LoadTextureFromFile("WaterNormalMap", "../Res/Assets/Textures/GreenWorld/WaterNormalMap.png");

        //Heightmap(s)
        Engine::ResourceManager::LoadHeightmap("TerrainHeightmap", "../Res/Assets/Textures/GreenWorld/Heightmap128.bmp");
    }

    void GreenWorldApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        Engine::Window::Init("GreenWorld");
        Engine::Camera3D::Init(glm::vec3(-75.0f, 74.0f, 70.0f), 0.4f, -23.0f, 25.0f);
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init();

        //Configure some application settings
        Engine::APP_SETTINGS.planeSize = 128;
        Engine::APP_SETTINGS.gravity   = -20.0f;

        //Load up shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer  = Engine::RenderManager::AddScene(_nearPlane, _farPlane, _lightPos, _lightCol);
        _shadowRenderer = Engine::RenderManager::AddShadows(8192, _lightPos, "ShadowCreateShader");
        _spriteRenderer = Engine::RenderManager::AddSprites();
        _waterRenderer  = Engine::RenderManager::AddWater();
        _smokeRenderer  = Engine::RenderManager::AddParticles
        (
            glm::vec3(87.0f, 34.0f, 92.5f),                                 //Spawn point
            200,                                                            //Number of particles
            5.0f,                                                           //Size
            0.05f,                                                          //Speed
            0.0f,                                                           //Gravity compliance
            6.0f,                                                           //Lifetime
            44.0f,                                                          //Respawn threshold (Y-Position)
            "ParticleTextureAtlas",                                         //Texture atlas
            "ParticleShader"                                                //Shader
        );

        //Set default shaders for the scene
        _sceneRenderer->SetTerrainShader("TerrainShader");
        _sceneRenderer->SetModelShader("ModelShader");
        _sceneRenderer->SetWaterShader("WaterShader");

        //Create UI and init audio system
        _interface = Engine::MakeScope<GreenWorldInterface>();
        _audio     = Engine::MakeScope<Engine::Audio>();
        _audio->PlaySound2D("../Res/Assets/Audio/GreenWorld/Music/TrueBlueSky.wav", true, 0.2f);
        _audio->PlaySound3D("../Res/Assets/Audio/GreenWorld/Sounds/River.wav", true, 4.0f, glm::vec3(39.0f, 14.0f, 56.0f), 0.5f);
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
            Engine::APP_SETTINGS.planeSize,                                 //Length in x direction
            Engine::APP_SETTINGS.planeSize,                                 //Length in z direction
            1.0f,                                                           //Tile size
            glm::vec3(0.0f, -2.7f, 0.0f),                                   //Position
            _shadowRenderer->GetDepthTexture(),                             //Depth texture
            "TerrainGrassTexture",                                          //Main texture
            "TerrainColormap",                                              //Texture for coloring
            "TerrainHeightmap"                                              //Heightmap
        );

        //Water
        _sceneRenderer->AddWater
        (
            Engine::APP_SETTINGS.planeSize - 112,                           //Length in x direction
            Engine::APP_SETTINGS.planeSize,                                 //Length in z direction
            1.0f,                                                           //Tile size
            glm::vec3(30.5f, 0.0f, 0.0f),                                   //Position
            0.025f,                                                         //Wave speed
            "WaterDuDvMap",                                                 //DuDv map
            "WaterNormalMap",                                               //Normal map
            _waterRenderer->GetReflectTexture(),                            //Reflect texture
            _waterRenderer->GetRefractTexture(),                            //Refract texture
            _waterRenderer->GetRefractDepthTexture()                        //Refract depth texture
        );

        //House
        _sceneRenderer->AddObject
        (
            1.2f,                                                           //Size
            glm::vec3(0.0f, -70.0f, 0.0f),                                  //Rotation
            glm::vec3(85.0f, 0.45f, 95.0f),                                 //Position
            _shadowRenderer->GetDepthTexture(),                             //Depth texture
            "House",                                                        //Name
            "../Res/Assets/Models/GreenWorld/House"                         //Path to obj file
        );

        //Bridge
        _sceneRenderer->AddObject
        (
            1.0f,                                                           //Size
            glm::vec3(0.0f),                                                //Rotation
            glm::vec3(39.0f, -0.45f, 47.0f),                                //Position
            _shadowRenderer->GetDepthTexture(),                             //Depth texture
            "Bridge",                                                       //Name
            "../Res/Assets/Models/GreenWorld/Bridge"                        //Path to obj file
        );

        //Tree
        _sceneRenderer->AddObject
        (
            1.0f,                                                           //Size
            glm::vec3(0.0f),                                                //Rotation
            glm::vec3(85.0f, 0.3f, 20.0f),                                  //Position
            _shadowRenderer->GetDepthTexture(),                             //Depth texture
            "Tree",                                                         //Name
            "../Res/Assets/Models/GreenWorld/Tree"                          //Path to obj file
        );
    }

    void GreenWorldApp::AddSprites()
    {
        //Shadow sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(210.0f, 0.0f),                                        //Position
            _shadowRenderer->GetDepthTexture(),                             //Texture
            Engine::ResourceManager::GetShader("SpriteShaderGreyscale")     //Shader
        );

        //Reflect sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(410.0f, 0.0f),                                        //Position
            _waterRenderer->GetReflectTexture(),                            //Texture
            Engine::ResourceManager::GetShader("SpriteShader")              //Shader
        );

        //Refract sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(610.0f, 0.0f),                                        //Position
            _waterRenderer->GetRefractTexture(),                            //Texture
            Engine::ResourceManager::GetShader("SpriteShader")              //Shader
        );

        //Refract depth sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(810.0f, 0.0f),                                        //Position
            _waterRenderer->GetRefractDepthTexture(),                       //Texture
            Engine::ResourceManager::GetShader("SpriteShaderGreyscale")     //Shader
        );
    }

    // ----- Public -----

    GreenWorldApp::GreenWorldApp()
    {
        InitModules();
        AddObjects();
        AddSprites();
    }

    GreenWorldApp::~GreenWorldApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
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

            Engine::RenderManager::RenderShadows();
        }

        {
            Engine::PROFILE_SCOPE("Render water");

            Engine::RenderManager::RenderWater();
        }

        {
            Engine::PROFILE_SCOPE("Render scene");

            Engine::RenderManager::RenderScene();
            Engine::RenderManager::RenderParticles();

            if(Engine::APP_SETTINGS.debugSprites)
                Engine::RenderManager::RenderSprites();
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