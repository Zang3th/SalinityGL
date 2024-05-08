#include "LiquefiedApp.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("GridShader", "../Res/Shader/Liquefied/Grid_VS.glsl", "../Res/Shader/Liquefied/Grid_FS.glsl");
    }

    void LiquefiedApp::AddBorderCells() const
    {
        for(Engine::uint32 x = 0; x < Engine::LiquiefiedParams::SIMULATION_WIDTH; x++)
        {
            for(Engine::uint32 y = 0; y < Engine::LiquiefiedParams::SIMULATION_HEIGHT; y++)
            {
                if((x == 0) || (y == 0) ||
                   (x == Engine::LiquiefiedParams::SIMULATION_WIDTH-1) ||
                   (y == Engine::LiquiefiedParams::SIMULATION_HEIGHT-1))
                {
                    //Add solid cell to simulation grid
                    _fluidSimulator->AddBorderCell(x, y);

                    //Add cell to renderer
                    _gridRenderer->Set(x, y, glm::vec3(1.0f, 1.0f, 1.0f));
                }
            }
        }
    }

    Engine::uint32 LiquefiedApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("Liquefied") != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        Engine::RenderManager::Init2D();

        //Load shaders and textures
        LoadResources();

        //Create grid renderer
        _gridRenderer = Engine::RenderManager::AddGridRenderer
        (
            Engine::LiquiefiedParams::SIMULATION_WIDTH,
            Engine::LiquiefiedParams::SIMULATION_HEIGHT,
            10,
            "GridShader"
        );

        //Create UI
        _interface = Engine::MakeScope<LiquefiedInterface>();

        //Create fluid simulator
        _fluidSimulator = Engine::MakeScope<Engine::FluidSimulator>();

        //Create timer
        _physicsTimer = Engine::MakeScope<Engine::Timer>(8);   //8ms
        _inputTimer   = Engine::MakeScope<Engine::Timer>(100); //100ms

        //Add border cells to simulation and renderer
        AddBorderCells();
        _gridRenderer->SetConfigAsDefault();

        return EXIT_SUCCESS;
    }

    void LiquefiedApp::UpdateTimer() const
    {
        const double dt_msec = Engine::Window::GetDeltaTime_msec();
        _physicsTimer->Update(dt_msec);
        _inputTimer->Update(dt_msec);
    }

    void LiquefiedApp::VisualizeSmoke() const
    {
        for(Engine::uint32 x = 1; x < Engine::LiquiefiedParams::SIMULATION_WIDTH-1; x++)
        {
            for(Engine::uint32 y = 1; y < Engine::LiquiefiedParams::SIMULATION_HEIGHT-1; y++)
            {
                const float val = _fluidSimulator->GetDensity(x, y);
                glm::vec3 color = {val, val, val};

                if(Engine::LiquiefiedParams::scientificColorScheme)
                {
                    color = Engine::Utility::GetScienticColor(val, 0.0f, 1.0f);
                }

                _gridRenderer->Set(x, y, color);
            }
        }
    }

    // ----- Public -----

    LiquefiedApp::LiquefiedApp()
    {
        if(InitModules() != EXIT_SUCCESS)
        {
            _initSuccess = false;
        }
        else
        {
            _initSuccess = true;
        }
    }

    LiquefiedApp::~LiquefiedApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
        Engine::Window::Close();
    }

    void LiquefiedApp::Update()
    {
        {
            Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
            UpdateTimer();

            //Handle input only if timer elapsed
            if(_inputTimer->CheckElapsedAndReset())
            {
                ProcessInput();
            }
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
            _interface->PrepareFrame();
        }

        {
            if(Engine::LiquiefiedParams::resetSimulation)
            {
                _fluidSimulator->Reset();
                AddBorderCells();
                Engine::LiquiefiedParams::resetSimulation = false;
            }

            if(!Engine::LiquiefiedParams::pauseSimulation)
            {
                //Check if timer elapsed
                if(_physicsTimer->CheckElapsedAndReset())
                {
                    const float dt = (float)Engine::Window::GetDeltaTime_sec();

                    //Add a horizontal turbine (initial velocity)
                   _fluidSimulator->AddHorizonalTurbine(1, 50, (float)Engine::LiquiefiedParams::turbinePower, dt);

                    //Run simulation timestep and visualize result
                    _fluidSimulator->TimeStep(dt);
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Visualize grid");

            if(Engine::LiquiefiedParams::visualizeSmoke)
            {
                VisualizeSmoke();
                _gridRenderer->UpdateGpuStorage();
            }
            else
            {
                _gridRenderer->UploadDefaultConfig();
            }

            _gridRenderer->Flush(nullptr);
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

    void LiquefiedApp::ProcessInput()
    {
        if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            Engine::UIParams::wireframeRendering = !Engine::UIParams::wireframeRendering;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            Engine::LiquiefiedParams::pauseSimulation = !Engine::LiquiefiedParams::pauseSimulation;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            Engine::LiquiefiedParams::visualizeSmoke = !Engine::LiquiefiedParams::visualizeSmoke;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_C) == GLFW_PRESS)
            Engine::LiquiefiedParams::scientificColorScheme = !Engine::LiquiefiedParams::scientificColorScheme;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_R) == GLFW_PRESS)
            Engine::LiquiefiedParams::resetSimulation = true;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            Engine::LiquiefiedParams::activateDebugging = !Engine::LiquiefiedParams::activateDebugging;
    }
}
