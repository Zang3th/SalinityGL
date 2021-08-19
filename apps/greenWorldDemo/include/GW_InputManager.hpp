#pragma once

#include "WindowManager.hpp"
#include "Camera.hpp"
#include "CoreDefines.hpp"

namespace GW
{
    class InputManager
    {
        private:
            static const Core::WindowManager*   _window;
            static       Core::Camera*          _camera;
            inline static double                _lastX          = double();
            inline static double                _lastY          = double();
            inline static bool                  _windowInFocus  = bool();

            static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        public:
            InputManager() = delete;
            static void Init(const Core::WindowManager* window, Core::Camera* camera);
            static void ProcessInput();
    };
}