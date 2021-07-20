#include "greenWorldApp.hpp"

// ----- Public -----

GreenWorldApp::GreenWorldApp()
{
    _windowManager = new WindowManager("GreenWorld Demo Application");
    Texture texture("../res/textures/greenWorld/Stone.jpg");
    Shader shader("../res/shader/greenWorld/standard_vs.glsl", "../res/shader/greenWorld/standard_fs.glsl");
}

GreenWorldApp::~GreenWorldApp()
{
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