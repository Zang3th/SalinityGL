#include "App.hpp"

namespace Engine
{
    // ----- Public -----

    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }
}