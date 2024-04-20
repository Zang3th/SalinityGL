#include "App.hpp"

namespace Engine
{
    // ----- Public -----

    App::~App() = default;

    bool App::GetInitSuccess() const
    {
        return _initSuccess;
    }

    bool App::IsRunning() const
    {
        return Engine::Window::IsRunning();
    }
}