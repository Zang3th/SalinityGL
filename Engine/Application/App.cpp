#include "App.hpp"

namespace Engine
{
    AppSettings APP_SETTINGS;

    // ----- Public -----

    App::~App() = default;

    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }
}