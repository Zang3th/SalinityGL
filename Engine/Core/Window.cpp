#include "Window.hpp"

namespace Engine
{
    // ----- Public -----

    uint32 Window::Init(const std::string& title)
    {
        //Initialize GLFW
        if(!glfwInit())
        {
            Logger::Error("Failed", "GLFW-Library", std::to_string(glfwGetError(nullptr)));
            return EXIT_FAILURE;
        }

        Logger::Info("Loaded", "GLFW-Library");

        //Set window hints
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        //Create window
        _windowName = title;
        _window = glfwCreateWindow(WindowParams::WIDTH, WindowParams::HEIGHT, _windowName.c_str(), nullptr, nullptr);

        if(!_window)
        {
            Logger::Error("Failed", "GLFW-Window", std::to_string(glfwGetError(nullptr)));
            return EXIT_FAILURE;
        }

        Logger::Info("Created", "GLFW-Window", _windowName);

        glfwMakeContextCurrent(_window);
        glfwSwapInterval(0);

        //Load OpenGL-Functions via glad
        if(!gladLoadGL(glfwGetProcAddress))
        {
            Logger::Error("Failed", "OpenGL-Load.");
            return EXIT_FAILURE;
        }

        //Log version
        std::string rendererString(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        std::string versionString(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        Logger::Info("Loaded", "OpenGL", std::string(rendererString + ", " + versionString));

        GLRenderSettings::SetViewport(WindowParams::WIDTH, WindowParams::HEIGHT);
        GLRenderSettings::EnableDebugging();

        //Start application
        _isRunning = true;

        return EXIT_SUCCESS;
    }

    void Window::Close()
    {
        glfwTerminate();
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
            Logger::Info("Closed", "GLFW-Window", _windowName);
        }

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
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

    float Window::GetAspectRatio()
    {
        return (float)WindowParams::WIDTH/(float)WindowParams::HEIGHT;
    }
}