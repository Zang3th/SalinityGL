#include "CellSimApp.hpp"

namespace CS
{
    // ----- Private -----

    void CellSimApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("ShadowCreateShader", "../Res/Shader/GreenWorld/ShadowCreate_VS.glsl", "../Res/Shader/GreenWorld/ShadowCreate_FS.glsl");
        Engine::ResourceManager::LoadShader("ModelShader", "../Res/Shader/GreenWorld/Model_VS.glsl", "../Res/Shader/GreenWorld/Model_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShaderGreyscale", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/SpriteGreyscale_FS.glsl");
        Engine::ResourceManager::LoadShader("CellShader", "../Res/Shader/CellSim/Cell_VS.glsl", "../Res/Shader/CellSim/Cell_FS.glsl");
    }

    void CellSimApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        Engine::Window::Init("CellSim");
        Engine::Camera3D::Init(glm::vec3(408.0f, 85.0f, 430.0f), 47.0f, -24.0f, 50.0f);
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init();

        //Configure some application settings
        Engine::AppSettings::planeSize = 1;

        //Load up shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer  = Engine::RenderManager::AddScene(_nearPlane, _farPlane, _lightPos, _lightCol);
        Engine::RenderManager::AddCells
        (
            1.0f,
            _nearPlane,
            _farPlane,
            "CellShader",
            glm::vec3(512.0f, 0.5f, 512.0f)
        );
        _shadowRenderer = Engine::RenderManager::AddShadows
        (
            8192,
            _lightPos,
            _lightTarget,
            glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, 77.0f, 240.0f),
            "ShadowCreateShader"
        );
        _spriteRenderer = Engine::RenderManager::AddSprites();

        //Set default shaders for the scene
        _sceneRenderer->SetModelShader("ModelShader");

        //Create UI
        _interface = Engine::MakeScope<CellSimInterface>();
    }

    void CellSimApp::AddObjects()
    {
        //Ground plane
        _sceneRenderer->AddPlane
        (
            Engine::AppSettings::planeSize,
            Engine::AppSettings::planeSize,
            1024.0f,
            glm::vec3(0.0f),
            _shadowRenderer->GetDepthTexture(),
            {}
        );

        //Frame
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(512.0f, 0.5f, 512.0f),
            _shadowRenderer->GetDepthTexture(),
            "Frame",
            "../Res/Assets/Models/CellSim/Frame"
        );

        //Cube
        //_sceneRenderer->AddObject
        //(
        //    0.5f,
        //    glm::vec3(0.0f),
        //    glm::vec3(500.0f, 10.0f, 524.0f),
        //    _shadowRenderer->GetDepthTexture(),
        //    "Cube",
        //    "../Res/Assets/Models/CellSim/Cube"
        //);
    }

    void CellSimApp::AddSprites()
    {
        //Shadow sprite
        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(0.0f, 0.0f),
            _shadowRenderer->GetDepthTexture(),
            Engine::ResourceManager::GetShader("SpriteShaderGreyscale")
        );
    }

    // ----- Public -----

    CellSimApp::CellSimApp()
    {
        InitModules();
        AddObjects();
        AddSprites();
    }

    CellSimApp::~CellSimApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
    }

    void CellSimApp::Update()
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

        {
            Engine::PROFILE_SCOPE("Render scene");

            Engine::RenderManager::RenderScene();
            Engine::RenderManager::RenderCells();

            if(Engine::AppSettings::debugSprites)
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