#include "greenWorldApp.hpp"

// ----- Public -----

GreenWorldApp::GreenWorldApp()
{
    _windowManager = new WindowManager("GreenWorld Demo Application");
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

    _windowManager->SwapBuffer();
}