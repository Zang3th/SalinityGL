#include "Window.hpp"

namespace Engine
{
    // ----- Public -----

    void Window::Init(const std::string& title)
    {
        if(!glfwInit())
            LOG(ERROR) << "Failed:   GLFW-Initialization | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Loaded:   GLFW";

        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        _windowName = title;
        _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, _windowName.c_str(), nullptr, nullptr);

        if(!_window)
            LOG(ERROR) << "Failed:   GLFW-Window-Creation | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Created:  GLFW-Window (" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << ")";

        glfwMakeContextCurrent(_window);
        glfwSwapInterval(1);

        if(!gladLoadGL())
            LOG(ERROR) << "Failed:   OpenGL-Loading with glad";
        else
            LOG(INFO) << "Loaded:   OpenGL with glad | " << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION);

        GLRenderSettings::SetViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        GLRenderSettings::EnableDebugging();

        //Start application
        _isRunning = true;
    }

    void Window::CalcFrametime()
    {
        //Calculate frametime
        double currentFrame = glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        //Accumulate to average the fps
        _frameCounter++;
        _dtAccumulated += _deltaTime;

        if(_frameCounter > 160)
        {
            _fpsAvg = 1 / (_dtAccumulated / _frameCounter);

            //Reset
            _frameCounter = 0;
            _dtAccumulated = 0.0f;
        }
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::ProcessEvents()
    {
        if(glfwWindowShouldClose(_window))
        {
            _isRunning = false;
            LOG(INFO) << "Closed:   " << _windowName;
        }

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
            _isRunning = false;
            LOG(INFO) << "Closed:   " << _windowName;
        }
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }

    bool Window::IsRunning()
    {
        return _isRunning;
    }

    double Window::GetDeltaTime()
    {
        return _deltaTime;
    }

    double Window::GetFps()
    {
        return _fpsAvg;
    }

    uint32 Window::GetFrameCounter()
    {
        return _frameCounter;
    }

    GLFWwindow* Window::GetWindow()
    {
        return _window;
    }
}