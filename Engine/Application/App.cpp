#include "App.hpp"

namespace Engine
{
    // ----- Public -----

    App::~App() = default;

    bool App::IsRunning() const
    {
        return Engine::Window::IsRunning();
    }
}
