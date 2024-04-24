#include "LiquefiedApp.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("SpriteShader", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/Sprite_FS.glsl");

        //Texture
        Engine::ResourceManager::LoadTextureToBuffer("BG_Texture", "../Res/Assets/Textures/Liquefied/Dark_Squares_1500.jpg");
    }

    Engine::uint32 LiquefiedApp::InitModules()
    {
        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("Liquefied") != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        Engine::RenderManager::Init();

        //Load shaders and textures
        LoadResources();

        //Create pixel renderer
        _pixelRenderer = Engine::RenderManager::AddPixelRenderer
        (
            Engine::LiquiefiedParams::SIMULATION_WIDTH,
            Engine::LiquiefiedParams::SIMULATION_HEIGHT,
            10,
            "BG_Texture",
            "SpriteShader"
        );

        //Create UI
        _interface = Engine::MakeScope<LiquefiedInterface>();

        //Create fluid simulator
        _fluidSimulator = Engine::MakeScope<Engine::FluidSimulator>();

        return EXIT_SUCCESS;
    }

    void LiquefiedApp::VisualizeSmoke() const
    {
        Engine::StaggeredGrid* grid = _fluidSimulator->GetGrid();

        for(Engine::uint32 x = 0; x < grid->width; x++)
        {
            for(Engine::uint32 y = 0; y < grid->height; y++)
            {
                const float val = grid->smoke_At(x, y);
                glm::vec3 color = {val, val, val};

                if(Engine::LiquiefiedParams::scientificColorScheme)
                {
                    color = Engine::Utility::GetScienticColor(val, 0.0f, 1.0f);
                }

                //Way to slow...
                //ToDo: Build SpriteInstancer
                //_pixelRenderer->Set(x, y, color);
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
            _timeElapsed += Engine::Window::GetDeltaTime();
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
            _interface->PrepareFrame();
        }

        {
            Engine::PROFILE_SCOPE("Simulate liquid");

            //Check if 10ms have elapsed
            if(_timeElapsed >= 0.01)
            {
                //Reset time
                _timeElapsed = 0;

                //Run simulation timestep and visualize result
                _fluidSimulator->TimeStep();
            }
        }

        {
            Engine::PROFILE_SCOPE("Render pixels");

            Engine::RenderManager::RenderPixels();
            VisualizeSmoke();

            //Test of the pixel renderer
            /*if(Engine::Window::GetFrameCounter() == 0)
            {
                _pixelRenderer->SetScreen(COLOR_WHITE);
            }
            else if(Engine::Window::GetFrameCounter() == 80)
            {
                _pixelRenderer->ClearScreen();
            }*/
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
