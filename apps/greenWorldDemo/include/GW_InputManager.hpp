#pragma once

#include "Window.hpp"
#include "Camera.hpp"
#include "CoreDefines.hpp"

namespace GW
{
    class InputManager
    {
        private:
            static const Core::Window*          _window;
            static       Core::Camera*          _camera;
            inline static double                _lastX          = double();
            inline static double                _lastY          = double();
            inline static bool                  _windowInFocus  = bool();

            static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
            static void MouseButtonCallback(GLFWwindow* window, Core::int32 button, Core::int32 action, Core::int32 mods);

        public:
            InputManager() = delete;
            static void Init(const Core::Window* window, Core::Camera* camera);
            static void ProcessInput();
    };
}