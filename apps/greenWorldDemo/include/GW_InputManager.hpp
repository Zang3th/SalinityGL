#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/Camera.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"

namespace GW
{
    class InputManager
    {
        private:
            static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

            static Core::WindowManager* _window;
            static Core::Camera* _camera;
            static bool _windowInFocus;
            static double _lastX, _lastY;

        public:
            InputManager() = delete;
            static void Init(Core::WindowManager* window, Core::Camera* camera);
            static void ProcessInput();
    };
}