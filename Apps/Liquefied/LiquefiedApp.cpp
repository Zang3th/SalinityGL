#include "LiquefiedApp.hpp"

namespace Liq
{
    // ----- Private -----

    void LiquefiedApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("SpriteShader", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/Sprite_FS.glsl");

        //Texture
        Engine::ResourceManager::LoadTextureToBuffer("BG_Texture", "../Res/Assets/Textures/Liquefied/Dark_Squares.jpg");
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

        //Create PixelRenderer
        _pixelRenderer = Engine::RenderManager::AddPixelRenderer("BG_Texture", "SpriteShader");

        //Create UI
        _interface = Engine::MakeScope<LiquefiedInterface>();

        return EXIT_SUCCESS;
    }

    // ----- Public -----

    LiquefiedApp::LiquefiedApp()
    {
        if(InitModules() != EXIT_SUCCESS)
        {
            appStartSuccess = false;
        }
        else
        {
            appStartSuccess = true;
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
            _interface->PrepareFrame();
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
        }

        {
            Engine::PROFILE_SCOPE("Render pixels");

            Engine::RenderManager::RenderPixels();

            //Test of the pixel renderer
            if(Engine::Window::GetFrameCounter() == 75)
            {
                _pixelRenderer->Set(0, 0, COLOR_WHITE);
                _pixelRenderer->Set(1919, 0, COLOR_RED);
                _pixelRenderer->Set(0, 1079, COLOR_BLUE);
                _pixelRenderer->Set(1919, 1079, COLOR_GREEN);
            }
            else if(Engine::Window::GetFrameCounter() == 0)
            {
                _pixelRenderer->Reset(0, 0);
                _pixelRenderer->Reset(1919, 0);
                _pixelRenderer->Reset(0, 1079);
                _pixelRenderer->Reset(1919, 1079);
            }
        }

        {
            Engine::PROFILE_SCOPE("Render UI");

            //_interface->AddElements();
            _interface->Render();
        }

        {
            Engine::PROFILE_SCOPE("End frame");

            Engine::Window::SwapBuffers();
        }
    }
}