#include "greenWorldApp.hpp"

// ----- Private -----

void GreenWorldApp::LoadResources()
{
    _resourceManager.LoadTexture("StoneTexture", "../res/textures/greenWorld/Stone.jpg");
    _resourceManager.LoadShader("SpriteShader", "../res/shader/greenWorld/sprite_vs.glsl", "../res/shader/greenWorld/sprite_fs.glsl");
}

// ----- Public -----

GreenWorldApp::GreenWorldApp()
{
    //Window-Settings
    _windowManager = std::make_shared<Core::WindowManager>();
    _windowManager->SetWindowTitle("GreenWorld Demo Application");

    //Renderer
    _renderer = std::make_shared<Core::Renderer>();

    //UI
    _userInterface = std::make_unique<GreenWorldInterface>(_windowManager, _renderer);

    //Resources
    LoadResources();

    //Create test sprite
    _testSprite = std::make_shared<Core::Sprite>(
        _resourceManager.GetTexture("StoneTexture"),
        _resourceManager.GetShader("SpriteShader"),
        glm::vec2(0.0f, 150.0f),
        glm::vec2(800.0f, 800.0f),
        0.0f,
        glm::vec3(0.37f, 0.77, 0.29f)
    );
}

bool GreenWorldApp::IsRunning()
{
    return _windowManager->WindowIsRunning();
}

void GreenWorldApp::Update()
{
    //Prepare frame
    _windowManager->PrepareFrame();    
    _userInterface->PrepareFrame();

    //Render stuff
    _renderer->Prepare();
    _renderer->Submit(_testSprite);
    _renderer->Flush();
    _userInterface->AddElements();
    _userInterface->Render();

    //Move sprite
    _testSprite->Translate(glm::vec2(30.0f * _windowManager->GetDeltaTime(), 0.0f));

    //End frame
    _windowManager->SwapBuffers();
}