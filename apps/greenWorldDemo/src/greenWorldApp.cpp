#include "greenWorldApp.hpp"

// ----- Private -----

void GreenWorldApp::LoadResources()
{
    _resourceManager.LoadTexture("StoneTexture", "../res/textures/greenWorld/Stone.jpg");
    _resourceManager.LoadShader("StandardShader", "../res/shader/greenWorld/standard_vs.glsl", "../res/shader/greenWorld/standard_fs.glsl");
}

// ----- Public -----

GreenWorldApp::GreenWorldApp()
{
    //Window-Settings
    _windowManager.SetWindowTitle("GreenWorld Demo Application");

    //Resources
    LoadResources();

    //Create test sprite
    _testSprite = std::make_unique<Core::Sprite>(
        _resourceManager.GetTexture("StoneTexture"),
        _resourceManager.GetShader("StandardShader"),
        glm::vec2(500.0f, 500.0f),
        glm::vec2(300.0f, 300.0f),
        0.0f,
        glm::vec3(0.37f, 0.77, 0.29f)
    );
}

bool GreenWorldApp::IsRunning()
{
    return _windowManager.WindowIsRunning();
}

void GreenWorldApp::Update()
{
    _windowManager.UpdateWindow();

    //Render stuff

    _windowManager.SwapBuffers();
}