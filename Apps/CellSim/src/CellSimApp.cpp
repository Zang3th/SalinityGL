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

    Engine::uint32 CellSimApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("CellSim") != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        Engine::Camera3D::Init(_camStartPos, _camStartYaw, _camStartPitch, 75.0f);
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init();

        //Configure some application settings
        Engine::AppSettings::planeSize = 1;

        //Load up shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer  = Engine::RenderManager::AddSceneRenderer(_nearPlane, _farPlane, _lightPos, _lightCol);

        _cellRenderer = Engine::RenderManager::AddCellRenderer
        (
            1.0f,
            _nearPlane,
            _farPlane,
            "CellShader",
            glm::vec3(482.0f, 2.0f, 482.0f)
        );

        _shadowRenderer = Engine::RenderManager::AddShadowRenderer
        (
            8192,
            _lightPos,
            _lightTarget,
            glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, 105.0f, 228.0f),
            "ShadowCreateShader"
        );

        _spriteRenderer = Engine::RenderManager::AddSpriteRenderer();

        //Set default shaders for the scene
        _sceneRenderer->SetModelShader("ModelShader");

        //Create UI
        _interface = Engine::MakeScope<CellSimInterface>();

        return EXIT_SUCCESS;
    }

    void CellSimApp::AddObjects()
    {
        //Ground plane
        _sceneRenderer->AddPlane
        (
            Engine::AppSettings::planeSize,
            Engine::AppSettings::planeSize,
            1024.0f,
            glm::vec3(0.0f, -1.0f, 0.0f),
            _shadowRenderer->GetDepthTexture(),
            {}
        );

        //Frame
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(512.0f, 0.0f, 512.0f),
            _shadowRenderer->GetDepthTexture(),
            "Frame",
            "../Res/Assets/Models/CellSim/Frame"
        );
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
        if(InitModules() != EXIT_SUCCESS)
        {
            appStartSuccess = false;
        }
        else
        {
            appStartSuccess = true;
            AddObjects();
            AddSprites();
        }
    }

    CellSimApp::~CellSimApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
        Engine::Window::Close();
    }

    void CellSimApp::Update()
    {
        {
            Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
            Engine::CameraController3D::ProcessInput();

            if(Engine::AppSettings::resetCamera)
            {
                Engine::Camera3D::SetPosition(_camStartPos, _camStartYaw, _camStartPitch);
                Engine::AppSettings::resetCamera = false;
            }
        }

        {
            //ToDo: Add profiling

            Engine::AppSettings::cellsAlive = _cellRenderer->GetAliveCellAmount();

            if(Engine::AppSettings::spawnNewCell)
            {
                _cellRenderer->SpawnCell
                (
                    Engine::AppSettings::selectedCellType,
                    Engine::AppSettings::selectedCellAmount,
                    glm::vec3(Engine::AppSettings::selectedCellCoords[0],
                              Engine::AppSettings::selectedCellCoords[1],
                              Engine::AppSettings::selectedCellCoords[2])
                );

                Engine::AppSettings::spawnNewCell = false;
            }

            if(Engine::AppSettings::cellsAlive > 0)
            {
                _cellRenderer->CalculateCellPhysics();
            }
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
            {
                Engine::RenderManager::RenderSprites();
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