#include "GreenWorldApp.hpp"

//Initialize extern settings
const Engine::int32  Engine::WINDOW_WIDTH       = 1920;
const Engine::int32  Engine::WINDOW_HEIGHT      = 1080;
const Engine::uint32 Engine::PLANE_SIZE         = 128;
const float          Engine::GRAVITY            = -20.0f;

bool                Engine::WIREFRAME_RENDERING = false;
bool                Engine::DEBUG_SPRITES       = false;
Engine::RenderStats Engine::RENDER_STATS;


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
    }

    void GreenWorldApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        Engine::Window::Init("GreenWorld");
        Engine::Camera3D::Init(glm::vec3(-75.0f, 74.0f, 70.0f), 0.4f, -23.0f, 25.0f);
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init();

        //Load up shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer  = Engine::RenderManager::AddScene(_nearPlane, _farPlane, _lightPos, _lightCol);
        _shadowRenderer = Engine::RenderManager::AddShadows(8192, _lightPos, "ShadowCreateShader");
        _spriteRenderer = Engine::RenderManager::AddSprites();
        /*_waterRenderer  = Engine::RenderManager::AddWater(0.025f);
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
        );*/

        //Set default shaders
        _sceneRenderer->SetTerrainShader("TerrainShader");
        _sceneRenderer->SetModelShader("ModelShader");
        _sceneRenderer->SetWaterShader("WaterShader");

        //Create UI
        _interface = Engine::MakeScope<GreenWorldInterface>();
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
            Engine::PLANE_SIZE,                                             //Length in x direction
            Engine::PLANE_SIZE,                                             //Length in z direction
            1.0f,                                                           //Tile size
            glm::vec3(0.0f, -2.7f, 0.0f),                                   //Position
            _shadowRenderer->GetDepthTexture(),                             //Depth texture
            "../Res/Assets/Textures/GreenWorld/Heightmap128.bmp",           //Heightmap filepath
            "TerrainGrassTexture",                                          //Main texture
            "TerrainColormap"                                               //Texture for coloring
        );

        /*//Water
        _sceneRenderer.AddWater
        (
            Engine::PLANE_SIZE - 112,                                       //Length in x direction
            Engine::PLANE_SIZE,                                             //Length in z direction
            1.0f,                                                           //Tile size
            glm::vec3(30.5f, 0.0f, 0.0f),                                   //Position
            "DuDvMap",                                                      //DuDv map
            "NormalMap",                                                    //Normal map
            _waterRenderer.get(),                                           //Water renderer
            "WaterPlaneShader"                                              //Shader
        );*/

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

        /*//Reflect sprite
        _spriteRenderer.AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(410.0f, 0.0f),                                        //Position
            _waterRenderer->GetReflectTexture(),                            //Texture
            "SpriteShader"                                                  //Shader
        );

        //Refract sprite
        _spriteRenderer.AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(610.0f, 0.0f),                                        //Position
            _waterRenderer->GetRefractTexture(),                            //Texture
            "SpriteShader"                                                  //Shader
        );

        //Refract depth sprite
        _spriteRenderer.AddSprite
        (
            glm::vec2(200.0f, 200.0f),                                      //Size
            glm::vec2(810.0f, 0.0f),                                        //Position
            _waterRenderer->GetRefractDepthTexture(),                       //Texture
            "SpriteShaderGreyscale"                                         //Shader
        );*/
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
        }

        {
            Engine::PROFILE_SCOPE("Render shadows");

            Engine::RenderManager::RenderShadows();
        }

        /*{
            Engine::PROFILE_SCOPE("Render water");

            Engine::RenderManager::RenderWater();
        }*/

        {
            Engine::PROFILE_SCOPE("Render scene");

            Engine::RenderManager::RenderScene();
            /*Engine::RenderManager::RenderParticles();*/

            if(Engine::DEBUG_SPRITES)
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