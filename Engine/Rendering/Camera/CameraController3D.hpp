#pragma once

#include "Window.hpp"
#include "Camera.hpp"
#include "CoreDefines.hpp"

namespace Engine
{
    class CameraController3D
    {
        private:
            inline static const Window* _window        = nullptr;
            inline static       double  _lastX         = double();
            inline static       double  _lastY         = double();
            inline static       bool    _windowInFocus = bool();

            static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
            static void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);

        public:
            CameraController3D() = delete;
            static void Init(const Window* window);
            static void ProcessInput();
    };
}