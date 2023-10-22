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
        Engine::CameraParams::startPitch    = -33.0f;
        Engine::CameraParams::startPos      = glm::vec3(-115.0f, 105.0f, 5.0f);
        Engine::LightParams::position       = glm::vec3(250.0f, 250.0f, -250.0f);
        Engine::LightParams::target         = glm::vec3(0.0f, 0.0f, 0.0f);

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
        _spriteRenderer = Engine::RenderManager::AddSpriteRenderer();

        //Create cell manager and add cell renderer
        _cellManager = Engine::MakeScope<Engine::CellManager>();
        _cellManager->AddCellRenderer("CellShader", glm::vec3(-30.0f, 0.5f, -30.0f));

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
            64.0f,
            glm::vec3(-32.0f, 0.0f, -32.0f),
            nullptr,
            {}
        );

        //Frame
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            nullptr,
            "Frame",
            "../Res/Assets/Models/CellSim/Frame"
        );
    }

    void CellSimApp::AddSprites()
    {
        //Nothing here (yet)
    }

    void CellSimApp::HandleCellSpawn()
    {
        Engine::CellParams cellParams{{0, Engine::CellSimParams::selectedCellType},
                                      glm::u32vec3(Engine::CellSimParams::selectedCellCoords[0],
                                                   Engine::CellSimParams::selectedCellCoords[1],
                                                   Engine::CellSimParams::selectedCellCoords[2])};

        if(Engine::CellSimParams::selectedCellAmount == 1)
        {
            _cellManager->AddCell(cellParams);
        }
        else if(Engine::CellSimParams::createSpawner)
        {
            _cellManager->AddCellSpawner(cellParams);
        }
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
            Engine::PROFILE_SCOPE("Manage cells");

            //Check for cell spawn
            if(Engine::CellSimParams::spawnNewCell)
            {
                HandleCellSpawn();
                Engine::CellSimParams::spawnNewCell = false;
            }

            //Check for cell delete
            if(Engine::CellSimParams::deleteCells)
            {
                _cellManager->DeleteCells();
                Engine::CellSimParams::deleteCells = false;
            }

            //Check for cell spawner delete
            if(Engine::CellSimParams::deleteSpawners)
            {
                _cellManager->DeleteSpawners();
                Engine::CellSimParams::deleteSpawners = false;
            }

            //Check for debug print
            if(Engine::CellSimParams::printDebug)
            {
                _cellManager->PrintDebug();
                Engine::CellSimParams::printDebug = false;
            }
        }

        {
            Engine::PROFILE_SCOPE("Calculate physics");

            //Check if 5ms have elapsed
            if(_timeElapsed >= 0.005)
            {
                //Reset time and increase tick counter
                _timeElapsed = 0;
                _tickCounter++;

                //Do physics
                if(Engine::CellSimParams::cellsAlive > 0)
                {
                    _cellManager->CalculateCellPhysics();
                }

                //Add new cells for every existing spawner
                if(_tickCounter > 5)
                {
                    _cellManager->ResolveCellSpawners();
                    _tickCounter = 0;
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
            _interface->PrepareFrame();
        }

        {
            Engine::PROFILE_SCOPE("Render scene");

            Engine::RenderManager::RenderScene();

            if(Engine::WindowParams::debugSprites)
            {
                Engine::RenderManager::RenderSprites();
            }
        }

        {
            Engine::PROFILE_SCOPE("Render cells");

            Engine::RenderManager::RenderCells();
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