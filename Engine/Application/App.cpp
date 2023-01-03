#include "App.hpp"

namespace Engine
{
    // ----- Public -----

    App::~App() = default;

    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }
}