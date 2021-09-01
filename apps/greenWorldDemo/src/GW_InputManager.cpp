#include "GW_InputManager.hpp"

namespace GW
{
    // ----- Private -----

    const Core::Window* InputManager::_window;
          Core::Camera* InputManager::_camera;

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

    void InputManager::MouseButtonCallback(GLFWwindow* window, Core::int32 button, Core::int32 action, Core::int32 mods)
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

    void InputManager::Init(const Core::Window* window, Core::Camera* camera)
    {
        //Set static member variables
        _window = window;
        _camera = camera;
        _windowInFocus = false;

        //Set callbacks
        glfwSetCursorPosCallback(_window->GetWindow(), MousePosCallback);
        glfwSetMouseButtonCallback(_window->GetWindow(), MouseButtonCallback);
    }

    void InputManager::ProcessInput()
    {
        auto dt = (float)_window->GetDeltaTime();

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->FORWARD, dt);

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->BACKWARD, dt);

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->RIGHT, dt);

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->LEFT, dt);

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->UP, dt);

        if (glfwGetKey(_window->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            _camera->ProcessKeyboard(_camera->DOWN, dt);
    }
}