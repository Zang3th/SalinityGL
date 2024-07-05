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
        for(Engine::uint32 x = 0; x < Engine::LiquefiedParams::SIMULATION_WIDTH; x++)
        {
            for(Engine::uint32 y = 0; y < Engine::LiquefiedParams::SIMULATION_HEIGHT; y++)
            {
                if((x == 0) || (y == 0) ||
                   (x == Engine::LiquefiedParams::SIMULATION_WIDTH-1) ||
                   (y == Engine::LiquefiedParams::SIMULATION_HEIGHT-1))
                {
                    //Add solid cell to simulation grid
                    _fluidSimulator->AddBorderCell(x, y);

                    //Add cell to renderer
                    _gridRenderer->Set(x, y, glm::vec3(1.0f, 1.0f, 1.0f));
                }
            }
        }
    }

    bool LiquefiedApp::Init()
    {
        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("Liquefied") != EXIT_SUCCESS)
        {
            return false;
        }
        Engine::RenderManager::Init2D();

        //Load shaders and textures
        LoadResources();

        //Create grid renderer
        _gridRenderer = new Engine::GridRenderer
        (
            Engine::LiquefiedParams::SIMULATION_WIDTH,
            Engine::LiquefiedParams::SIMULATION_HEIGHT,
            10,
            "GridShader"
        );
        Engine::RenderManager::Submit(_gridRenderer);

        //Create UI
        _interface = Engine::MakeScope<LiquefiedInterface>();
        _interface->Init();

        //Create fluid simulator
        _fluidSimulator = Engine::MakeScope<Engine::FluidSimulator>();

        //Create timer
        _physicsTimer = Engine::MakeScope<Engine::Timer>(16);   //16ms
        _inputTimer   = Engine::MakeScope<Engine::Timer>(100);  //100ms

        //Add border cells to simulation and renderer
        AddBorderCells();
        _gridRenderer->SetConfigAsDefault();

        return true;
    }

    void LiquefiedApp::UpdateTimer() const
    {
        const double dt_msec = Engine::Window::GetDeltaTime_msec();
        _physicsTimer->Update(dt_msec);
        _inputTimer->Update(dt_msec);
    }

    void LiquefiedApp::VisualizeSmoke() const
    {
        glm::vec3 color{0.0f};

        for(Engine::uint32 x = 1; x < Engine::LiquefiedParams::SIMULATION_WIDTH-1; x++)
        {
            for(Engine::uint32 y = 1; y < Engine::LiquefiedParams::SIMULATION_HEIGHT-1; y++)
            {
                const float val = _fluidSimulator->GetDensity(x, y);

                if(Engine::LiquefiedParams::visualizationChoice == Engine::Visualization::Greyscale)
                {
                    color = {val, val, val};
                }
                else if(Engine::LiquefiedParams::visualizationChoice == Engine::Visualization::BlackBody)
                {
                    color = Engine::Utility::GetColor_BlackBody(val);
                }
                else if(Engine::LiquefiedParams::visualizationChoice == Engine::Visualization::ParaView)
                {
                    color = Engine::Utility::GetColor_ParaView(val);
                }

                _gridRenderer->Set(x, y, color);
            }
        }
    }

    // ----- Public -----

    LiquefiedApp::LiquefiedApp() = default;

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
            if(Engine::LiquefiedParams::resetSimulation)
            {
                _fluidSimulator->Reset();
                AddBorderCells();
                Engine::LiquefiedParams::resetSimulation = false;
            }

            if(!Engine::LiquefiedParams::pauseSimulation)
            {
                //Check if timer elapsed
                if(_physicsTimer->CheckElapsedAndReset())
                {
                    const float dt = (float)Engine::Window::GetDeltaTime_sec();

                    //Add a horizontal turbine (initial velocity)
                   _fluidSimulator->AddHorizonalTurbine(1, 50, (float)Engine::LiquefiedParams::turbinePower, dt);

                    //Run simulation timestep
                    _fluidSimulator->TimeStep(dt);
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Visualize grid");

            if(Engine::LiquefiedParams::visualizationChoice != Engine::Visualization::Nothing)
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
            Engine::LiquefiedParams::pauseSimulation = !Engine::LiquefiedParams::pauseSimulation;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_R) == GLFW_PRESS)
            Engine::LiquefiedParams::resetSimulation = true;

        else if(glfwGetKey(Engine::Window::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            Engine::LiquefiedParams::activateDebugging = !Engine::LiquefiedParams::activateDebugging;
    }
}
