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

    //UI
    _userInterface = std::make_unique<GreenWorldInterface>(_windowManager);

    //Resources
    LoadResources();

    //Create test sprite
    _testSprite = std::make_shared<Core::Sprite>(
        _resourceManager.GetTexture("StoneTexture"),
        _resourceManager.GetShader("SpriteShader"),
        glm::vec2(500.0f, 500.0f),
        glm::vec2(300.0f, 300.0f),
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
    _renderer.Prepare();
    _renderer.Submit(_testSprite);
    _renderer.Flush();
    _userInterface->AddElements();
    _userInterface->Render();

    //Move sprite
    _testSprite->Translate(glm::vec2(10.0f * _windowManager->GetDeltaTime(), 0.0f));

    //End frame
    _windowManager->SwapBuffers();
}