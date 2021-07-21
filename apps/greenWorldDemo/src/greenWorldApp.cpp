#include "greenWorldApp.hpp"

// ----- Public -----

GreenWorldApp::GreenWorldApp()
{
    _windowManager = new WindowManager("GreenWorld Demo Application");
    ResourceManager::LoadTexture("StoneTexture", "../res/textures/greenWorld/Stone.jpg");
    ResourceManager::LoadShader("StandardShader", "../res/shader/greenWorld/standard_vs.glsl", "../res/shader/greenWorld/standard_fs.glsl");
}

GreenWorldApp::~GreenWorldApp()
{
    ResourceManager::CleanUp();
    delete _windowManager;
}

bool GreenWorldApp::IsRunning()
{
    return _windowManager->WindowIsRunning();
}

void GreenWorldApp::Update()
{
    _windowManager->UpdateWindow();

    //Render stuff

    _windowManager->SwapBuffers();
}