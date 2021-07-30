#include "GW_InputManager.hpp"

namespace GW
{
    // ----- Private -----

    Core::Ref<Core::WindowManager> InputManager::_window;
    Core::Ref<Core::Camera> InputManager::_camera;
    bool InputManager::_windowInFocus;
    double InputManager::_lastX, InputManager::_lastY;

    void InputManager::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
    {
        if (_windowInFocus)
        {
            double xOffset = xPos - _lastX;
            double yOffset = _lastY - yPos; //Reversed since y-coordinates go from bottom to top

            _lastX = xPos;
            _lastY = yPos;
            _camera->ProcessMouseMovement((float)xOffset, (float)yOffset);
        }
        else
        {
            _lastX = xPos;
            _lastY = yPos;
        }
    }

    void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        //_lastY needs to be greater than 27 to not trigger on the titlebar
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && _lastY > 27)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            _windowInFocus = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            _windowInFocus = false;
        }
    }

    // ----- Public -----

    void InputManager::Init(Core::Ref<Core::WindowManager> window, Core::Ref<Core::Camera> camera)
    {
        //Set static member variables
        _window = window;
        _camera = camera;
        _windowInFocus = true;

        //Set callbacks
        glfwSetCursorPosCallback(_window->GetWindow(), MousePosCallback);
        glfwSetMouseButtonCallback(_window->GetWindow(), MouseButtonCallback);
    }
}