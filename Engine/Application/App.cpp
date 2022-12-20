#include "App.hpp"

namespace Engine
{
    bool App::IsRunning()
    {
        return Engine::Window::IsRunning();
    }
}