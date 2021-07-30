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

            static Core::Ref<Core::WindowManager> _window;
            static Core::Ref<Core::Camera> _camera;
            static bool _windowInFocus;
            static double _lastX, _lastY;

        public:
            InputManager() = delete;
            static void Init(Core::Ref<Core::WindowManager> window, Core::Ref<Core::Camera> camera);
    };
}