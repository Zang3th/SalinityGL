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
        //Configure some global application parameters
        Engine::RenderParams::farPlane      = 1024.0f;
        Engine::RenderParams::planeSize     = 1;
        Engine::CameraParams::movementSpeed = 75.0f;
        Engine::CameraParams::startYaw      = 38.0f;
        Engine::CameraParams::startPitch    = -29.0f;
        Engine::CameraParams::startPos      = glm::vec3(350.0f, 125.0f, 415.0f);
        Engine::LightParams::position       = glm::vec3(565.0f, 170.0f, 455.0f);
        Engine::LightParams::target         = glm::vec3(515.0f, 40.0f, 505.0f);

        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("CellSim") != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        Engine::Camera3D::Init();
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init();

        //Load shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer  = Engine::RenderManager::AddSceneRenderer();
        _sceneRenderer->SetModelShader("ModelShader");
        _shadowRenderer = Engine::RenderManager::AddShadowRenderer(8192, glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, 105.0f, 228.0f), "ShadowCreateShader");
        _cellRenderer   = Engine::RenderManager::AddCellRenderer(1.0f, "CellShader", glm::vec3(482.0f, 2.0f, 482.0f));
        _spriteRenderer = Engine::RenderManager::AddSpriteRenderer();

        //Create UI
        _interface = Engine::MakeScope<CellSimInterface>();

        return EXIT_SUCCESS;
    }

    void CellSimApp::AddObjects()
    {
        //Ground plane
        _sceneRenderer->AddPlane
        (
            Engine::RenderParams::planeSize,
            Engine::RenderParams::planeSize,
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
            _timeElapsed += Engine::Window::GetDeltaTime();

            if(Engine::WindowParams::resetCamera)
            {
                Engine::Camera3D::ResetPosition();
                Engine::WindowParams::resetCamera = false;
            }
        }

        {
            //ToDo: Add profiling

            //Check for cell spawn
            if(Engine::CellSimParams::spawnNewCell)
            {
                if(Engine::CellSimParams::selectedCellAmount > 0)
                {
                    _cellRenderer->SpawnCell
                    (
                        Engine::CellSimParams::selectedCellType,
                        Engine::CellSimParams::selectedCellAmount,
                        glm::u32vec3(Engine::CellSimParams::selectedCellCoords[0],
                                     Engine::CellSimParams::selectedCellCoords[1],
                                     Engine::CellSimParams::selectedCellCoords[2])
                    );
                }

                Engine::CellSimParams::spawnNewCell = false;
            }

            //Check for cell delete
            if(Engine::CellSimParams::deleteAllCells)
            {
                _cellRenderer->DeleteAllCells();
                Engine::CellSimParams::deleteAllCells = false;
            }

            Engine::CellSimParams::cellsAlive = _cellRenderer->GetAliveCellAmount();

            //Check if we need to do physics (every 10ms)
            if(Engine::CellSimParams::cellsAlive > 0 && _timeElapsed >= 0.01)
            {
                _cellRenderer->CalculateCellPhysics();
                _timeElapsed = 0;
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

            if(Engine::WindowParams::debugSprites)
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