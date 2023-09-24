#include "CameraController3D.hpp"

namespace Engine
{
    // ----- Private -----

    void CameraController3D::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
    {
        if(_windowInFocus)
        {
            double xOffset = xPos - _lastX;
            double yOffset = _lastY - yPos; //Reversed since y-coordinates go from bottom to top

            _lastX = xPos;
            _lastY = yPos;
            Camera3D::ProcessMouseMovement((float)xOffset, (float)yOffset);
        }
        else
        {
            _lastX = xPos;
            _lastY = yPos;
        }
    }

    void CameraController3D::MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods)
    {
        //_lastY needs to be greater than 27 to not trigger on the titlebar
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && _lastY > 27)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            _windowInFocus = true;
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            DeFocusWindow(window);
        }
    }

    // ----- Public -----

    void CameraController3D::Init()
    {
        _windowInFocus = false;

        //Set callbacks
        glfwSetCursorPosCallback(Window::GetWindow(), MousePosCallback);
        glfwSetMouseButtonCallback(Window::GetWindow(), MouseButtonCallback);
    }

    void CameraController3D::ProcessInput()
    {
        auto dt = (float)Window::GetDeltaTime();

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::FORWARD, dt);

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::BACKWARD, dt);

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::RIGHT, dt);

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::LEFT, dt);

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::UP, dt);

        if(glfwGetKey(Window::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            Camera3D::ProcessKeyboard(Camera3D::DOWN, dt);
    }

    void CameraController3D::DeFocusWindow(GLFWwindow* window)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _windowInFocus = false;
    }
}