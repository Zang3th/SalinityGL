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