#include "LiquefiedApp.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("GridShader", "../Res/Shader/Liquefied/Grid_VS.glsl", "../Res/Shader/Liquefied/Grid_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShader", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/Sprite_FS.glsl");

        //Textures
        // Engine::ResourceManager::LoadTextureBuffer("TurbineTexBuf", "../Res/Assets/Textures/Liquefied/Turbine_512.png");
        // Engine::ResourceManager::LoadTextureBuffer("ObstacleTexBuf", "../Res/Assets/Textures/Liquefied/Box_512.png");
        Engine::ResourceManager::LoadTextureBuffer("TestTexBuf", "../Res/Assets/Textures/Liquefied/Test_1C_1G_8Px.png");
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
                }
            }
        }
    }

    void LiquefiedApp::AddTurbine() const
    {
        for(Engine::uint32 x = 0; x < turbineSize; x++)
        {
            for(Engine::uint32 y = 0; y < turbineSize; y++)
            {
                _fluidSimulator->AddBorderCell(turbinePos.x + x, turbinePos.y + y);
            }
        }
    }

    void LiquefiedApp::AddObstacles() const
    {
        for(Engine::uint32 x = 0; x < obstacleSize; x++)
        {
            for(Engine::uint32 y = 0; y < obstacleSize; y++)
            {
                _fluidSimulator->AddBorderCell(obstaclePos.x + x, obstaclePos.y + y);
            }
        }
    }

    void LiquefiedApp::TurbinePushVelocity(const float dt) const
    {
       _fluidSimulator->AddHorizonalTurbine
        (
            turbineOutlet.x,
            turbineOutlet.y,
            (float)Engine::LiquefiedParams::turbinePower,
            dt
        );
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

        //Add border cells, turbine and physical obstacles to the simulation
        AddBorderCells();
        AddTurbine();
        AddObstacles();

        _spriteRenderer = new Engine::SpriteRenderer();
        Engine::RenderManager::Submit(_spriteRenderer);

        Engine::ResourceManager::CreateGLTextureFromBuffer("TestTexture", Engine::ResourceManager::GetTextureBuffer("TestTexBuf"));

        _spriteRenderer->AddSprite
        (
            glm::vec2(200.0f, 200.0f),
            glm::vec2(10.0f, 800.0f),
            Engine::ResourceManager::GetGLTexture("TestTexture"),
            Engine::ResourceManager::GetShader("SpriteShader")
        );

        //Add sprites/textures to the config of the renderer
        // _gridRenderer->AddTextureSubsampled
        // (
        //     "TurbineTexture",
        //     turbinePos,
        //     turbineSize
        // );
        _gridRenderer->AddTextureBufferSubsampled
        (
            "TestTexBuf",
            obstaclePos,
            obstacleSize
        );

        return true;
    }

    void LiquefiedApp::UpdateTimer() const
    {
        const double dt_msec = Engine::Window::GetDeltaTime_msec();
        _physicsTimer->Update(dt_msec);
        _inputTimer->Update(dt_msec);
    }

    void LiquefiedApp::RenderSmoke() const
    {
        glm::vec3 color{0.0f};

        for(Engine::uint32 x = 0; x < Engine::LiquefiedParams::SIMULATION_WIDTH; x++)
        {
            for(Engine::uint32 y = 0; y < Engine::LiquefiedParams::SIMULATION_HEIGHT; y++)
            {
                //Check for border cell (could be any type of object/obstacle)
                if(_fluidSimulator->GetBorder(x, y) == 0.0f)
                {
                    if(Engine::LiquefiedParams::renderObjects)
                    {
                        continue;
                    }

                    //Else overwrite objects/sprites/textures with default gray
                    // color = {0.5f, 0.5f, 0.5f};
                }
                else
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
                AddTurbine();
                AddObstacles();
                Engine::LiquefiedParams::resetSimulation = false;
            }

            if(!Engine::LiquefiedParams::pauseSimulation)
            {
                //Check if timer elapsed
                if(_physicsTimer->CheckElapsedAndReset())
                {
                    const float dt = (float)Engine::Window::GetDeltaTime_sec();
                    TurbinePushVelocity(dt);
                    _fluidSimulator->TimeStep(dt);
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Visualize grid");

            // RenderSmoke();
            _gridRenderer->UpdateGpuStorage();
            _gridRenderer->Flush(nullptr);
            _spriteRenderer->Flush(nullptr);
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
